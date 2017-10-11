#ifndef ONTCOMMANDS_ONTVERSION_H
#define ONTCOMMANDS_ONTVERSION_H

#include "OLTCommands_BaseInfo.h"

namespace OLTCommands
{
	class ONTVersion;
}

class ONTVersion : public OLTCommandReply
{
protected:
	struct ONTVersionInfo : public OntBasicInfo
	{
		QString vendorID;
		QString ontVersion;
		QString productID;
		QString equipmentID;
		QString mainSwVersion;
		QString standbySwVersion;
		QString suportXMLVersion;
		QString productDesc;
		void clear()
		{
			vendorID.clear();
			ontVersion.clear();
			productID.clear();
			equipmentID.clear();
			mainSwVersion.clear();
			standbySwVersion.clear();
			suportXMLVersion.clear();
			productDesc.clear();
		}
		virtual QStringList toStringInfoData() const
		{
			return OntBasicInfo::toStringInfoData()
					<< QObject::tr("ID del vendedor") << vendorID
					<< QObject::tr("Versión de la ONT") << ontVersion
					<< QObject::tr("ID del producto") << productID
					<< QObject::tr("ID del equipamiento") << equipmentID
					<< QObject::tr("Versión principal del software") << mainSwVersion
					<< QObject::tr("Versión del software en standby") << standbySwVersion
					<< QObject::tr("Versión del fichero XML de soporte") << suportXMLVersion
					<< QObject::tr("Descripción del producto") << productDesc;
		}
	};
	ONTVersionInfo m_versionInfo;
public:
	ONTVersion(const QString &label, const QString &command, const QString &rawData);
	virtual QStringList toStringInfoData() const
	{
		return m_versionInfo.toStringInfoData();
	}
};

#endif // ONTCOMMANDS_ONTVERSION_H
