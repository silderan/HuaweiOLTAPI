#include "QFrontend.h"
#include "ui_QFrontend.h"

#include <QMessageBox>

#include "QInfoDialog.h"
#include "GlobalConfig.h"

QFrontend::QFrontend(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::QFrontend)
{
	ui->setupUi(this);

	// List of widgets that has the param values.
	// All the widgets inside this list will be enabled/disabled
	// up to the command combo-box current index selected.
	m_paramWidgets.append( QList<QWidget*>()
						   << ui->serial
						   << ui->frame
						   << ui->slot
						   << ui->port
						   << ui->value
						   << ui->ontID );

	// This string list order must match with the enum CommanIndex
	// to the command combo box and send button works fine and easy
	// to add more commands while keaping easy to read code.
	// The QVariant parámeters stands for the enabled widgets object names.
	// All the widgets names listed here, will be enabled to allow user
	// to change values.
	ui->command->addItem( "set scroll", QStringList() << "value" );
	ui->command->addItem( "get unmanaged ONTs", QStringList() );
	ui->command->addItem( "get board info", QStringList() << "frame" << "slot" );
	ui->command->addItem( "get ont info", QStringList() << "frame" << "slot" << "port" << "ontID" );
	ui->command->addItem( "get ont WAN info", QStringList() << "frame" << "slot" << "port" << "ontID" );
	ui->command->addItem( "get ont MAC info", QStringList() << "frame" << "slot" << "port" << "ontID" );
	ui->command->addItem( "get GPON service profiles", QStringList() );
	ui->command->addItem( "get GPON service profile info", QStringList() << "serviceProfileID" );

	globalConfig.load();
	ui->leFQDN->setText( globalConfig.hostName() );
	ui->sbPort->setValue( globalConfig.hostPort() );
	ui->leUName->setText( globalConfig.oltUName() );
	ui->leUPass->setText( globalConfig.oltUPpass() );

	ui->command->setCurrentIndex( ui->command->findText(globalConfig.command()) );
	ui->serial->setText( globalConfig.serialNumber() );
	ui->value->setValue( globalConfig.valueNumber() );
	ui->frame->setValue( globalConfig.frameNumber() );
	ui->slot->setValue( globalConfig.slotNumber() );
	ui->port->setValue( globalConfig.portNumber() );
	ui->ontID->setValue( globalConfig.ontIDNumber() );

	// Connect all telnet comming data to the main text viewer.
	connect( &huaweiOLT, SIGNAL(newData(const char*,int)), this, SLOT(oltTelnetIncommings(const char*,int)) );
	// TO see telnet status.
	connect( &huaweiOLT, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(oltTelnetStatusChanged(QAbstractSocket::SocketState)) );
	connect( &huaweiOLT, SIGNAL(oltStateChanged(QTelnetInterface::OLTState)), this, SLOT(oltStatusChanged(QTelnetInterface::OLTState)) );
	connect( &huaweiOLT, SIGNAL(errorResponse(QString,QString,QString)), this, SLOT(oltErrorResponse(QString,QString,QString)) );
	connect( &huaweiOLT, SIGNAL(boardInfo(BoardInfo)), this, SLOT(boardInfoReceived(BoardInfo)) );
	connect( &huaweiOLT, SIGNAL(unmanagedOnts(UnmanagedONTs)), this, SLOT(unmanagedReceived(UnmanagedONTs)) );
	connect( &huaweiOLT, SIGNAL(ontWANInfo(ONTWANInfo)), this, SLOT(ontsWANInfoReceived(ONTWANInfo)) );
	connect( &huaweiOLT, SIGNAL(ontMACInfo(ONTMACInfo)), this, SLOT(ontsMACInfoReceived(ONTMACInfo)) );
	connect( &huaweiOLT, SIGNAL(gponServiceProfiles(GPONServiceProfiles)), this, SLOT(gponSrvPrfsReceived(GPONServiceProfiles)) );
	connect( &huaweiOLT, SIGNAL(gponServiceProfile(GPONServiceProfile)), this, SLOT(gponSrvPrfReceived(GPONServiceProfile)) );
}

QFrontend::~QFrontend()
{
	huaweiOLT.close();

	globalConfig.setHostName(ui->leFQDN->text());
	globalConfig.setHostUName(ui->leUName->text());
	globalConfig.sethostUPass(ui->leUPass->text());
	globalConfig.setHostPort(ui->sbPort->value());

	globalConfig.setCommand( ui->command->currentText() );
	globalConfig.setSerialNumber( ui->serial->text() );
	globalConfig.setValueNumber( ui->value->value() );
	globalConfig.setFrameNumber( ui->frame->value() );
	globalConfig.setSlotNumber( ui->slot->value() );
	globalConfig.setPortNumber( ui->port->value() );
	globalConfig.setONTIDNumber( ui->ontID->value() );

	globalConfig.save();
	delete ui;
	ui = Q_NULLPTR;
}

void QFrontend::addViewerText(const QString &text)
{
	if( ui )
		ui->telnetOut->textCursor().insertText(text);
}

void QFrontend::oltTelnetIncommings(const char *data, int length)
{
	addViewerText( QByteArray(data, length) );
}

void QFrontend::oltTelnetStatusChanged(QAbstractSocket::SocketState state)
{
	switch(state)
	{
	case QAbstractSocket::UnconnectedState:
		addViewerText( tr("Desconectado de %1\n").arg(huaweiOLT.peerInfo()) );
		ui->btLogin->setText( tr("Conectar") );
		break;
	case QAbstractSocket::HostLookupState:
		addViewerText( tr("Resolviendo DNS %1\n").arg(huaweiOLT.peerInfo()) );
		ui->btLogin->setText( tr("Cancelar") );
		break;
	case QAbstractSocket::ConnectingState:
		addViewerText( tr("Conectando a %1\n").arg(huaweiOLT.peerInfo()) );
		ui->btLogin->setText( tr("Cancelar") );
		break;
	case QAbstractSocket::ConnectedState:
		addViewerText( tr("Conectado a %1\n").arg(huaweiOLT.peerInfo()) );
		ui->btLogin->setText( tr("Desconectar") );
		break;
	case QAbstractSocket::BoundState:
		addViewerText( tr("Enlazado") );
		ui->btLogin->setText( tr("Cancelar") );
		break;
	case QAbstractSocket::ListeningState:
		addViewerText( tr("Escuchando") );
		ui->btLogin->setText( tr("Cancelar") );
		break;
	case QAbstractSocket::ClosingState:
		addViewerText( tr("Cerrando conexión de %1\n").arg(huaweiOLT.peerInfo()) );
		ui->btLogin->setText( tr("Forzar") );
		break;
	}
}

void QFrontend::oltStatusChanged(QTelnetInterface::OLTState state)
{
	switch( state )
	{
	case QTelnetInterface::OltUnknownSate:
		break;
	case QTelnetInterface::OltUnconnected:
		break;
	case QTelnetInterface::OltConnected:
		break;
	case QTelnetInterface::OltLogging:
		break;
	case QTelnetInterface::OltLogged:
		huaweiOLT.setConfigMode();
		break;
	case QTelnetInterface::OltAdminMode:
		break;
	case QTelnetInterface::OltConfigMode:
		break;
	}
}

void QFrontend::oltErrorResponse(const QString &tag, const QString &cmd, const QString &err)
{
	QMessageBox::warning(this, this->windowTitle(),
						 tr("Error reported from OLT while processing cmd \"%1\" Tag=\"%2\": \n%3").arg(cmd, tag, err));
	addViewerText(tr("Error on command [%1] %2; %3\n").arg(tag, cmd, err));
}

void QFrontend::on_btLogin_clicked()
{
	switch( huaweiOLT.state() )
	{
	case QAbstractSocket::UnconnectedState:
		huaweiOLT.connectToOLT(ui->leFQDN->text(),
							   ui->sbPort->value(),
							   ui->leUName->text(),
							   ui->leUPass->text());
		break;
	case QAbstractSocket::HostLookupState:
		huaweiOLT.disconnectFromHost();
		break;
	case QAbstractSocket::ConnectingState:
		huaweiOLT.disconnectFromHost();
		break;
	case QAbstractSocket::ConnectedState:
		huaweiOLT.disconnectFromHost();
		break;
	case QAbstractSocket::BoundState:
		huaweiOLT.close();
		break;
	case QAbstractSocket::ListeningState:
		huaweiOLT.close();
		break;
	case QAbstractSocket::ClosingState:
		huaweiOLT.close();
		break;
	}
}

void QFrontend::on_command_currentIndexChanged(int index)
{
	QStringList paramNames = ui->command->itemData(index).toStringList();
	foreach( QWidget *paramObject, m_paramWidgets )
		paramObject->setEnabled(paramNames.contains(paramObject->objectName()));
}

void QFrontend::on_sendCMD_clicked()
{
	switch( static_cast<CommandIndex>(ui->command->currentIndex()) )
	{
	case QFrontend::CmdScroll:
		huaweiOLT.setScroll(ui->value->value());
		break;
	case QFrontend::CmdGetUnmanaged:
		huaweiOLT.getUnmanaged();
		break;
	case QFrontend::CmdBoardInfo:
		huaweiOLT.getBoardInfo( ui->frame->value(), ui->slot->value() );
		break;
	case QFrontend::CmdONTInfo:
		break;
	case QFrontend::CmdONTWANInfo:
		huaweiOLT.getONTWANInfo( ui->frame->value(), ui->slot->value(), ui->port->value(), ui->ontID->value() );
		break;
	case QFrontend::CmdONTMACInfo:
		huaweiOLT.getONTMACInfo( ui->frame->value(), ui->slot->value(), ui->port->value(), ui->ontID->value() );
		break;
	case QFrontend::CmdGPONServiceProfiles:
		huaweiOLT.getGPONServiceProfiles();
		break;
	case QFrontend::CmdGPONServiceProfile:
		huaweiOLT.getGPONServiceProfile( ui->serviceProfileID->value() );
		break;
	}
}

void QFrontend::boardInfoReceived(const BoardInfo &boardInfo)
{
	QInfoDialog( tr("Board Info"), boardInfo.toString() ).exec();
}

void QFrontend::unmanagedReceived(const UnmanagedONTs &unmanaged)
{
	QInfoDialog( tr("Unmanaged ONTs"), unmanaged.toString() ).exec();
}

void QFrontend::ontsWANInfoReceived(const ONTWANInfo &ontWANInfo)
{
	QInfoDialog( tr("ONTs WAN Info"), ontWANInfo.toString() ).exec();
}

void QFrontend::ontsMACInfoReceived(const ONTMACInfo &ontMACInfo)
{
	QInfoDialog( tr("ONTs MAC Info"), ontMACInfo.toString() ).exec();
}

void QFrontend::gponSrvPrfsReceived(const GPONServiceProfiles &gponSrvProfiles)
{
	QInfoDialog( tr("GPON service-profiles"), gponSrvProfiles.toString() ).exec();
}

void QFrontend::gponSrvPrfReceived(const GPONServiceProfile &gponSrvProfile)
{
	QInfoDialog( tr("GPON service-profile"), gponSrvProfile.toString() ).exec();
}
