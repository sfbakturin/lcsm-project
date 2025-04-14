#ifndef LCSM_VISUAL_CORE_PROJECT_H
#define LCSM_VISUAL_CORE_PROJECT_H

#include <Core/CoreScene.h>
#include <GUI/GUIOptions.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/Verilog/Module.h>

#include <QDateTime>
#include <QHash>
#include <QString>
#include <map>
#include <memory>

class Project
{
  public:
	Project(const QString &name);
	~Project() noexcept = default;

	const QString &name() const noexcept;

	const QString &filename() const noexcept;
	void setFilename(const QString &filename);

	bool isSavePointDetermined() const noexcept;

	static std::unique_ptr< Project > fromFile(const QString &filename);

	bool isCircuitExists(const QString &name) const;
	bool isVerilogExists(const QString &name) const;

	QString createEmptyCircuit(const QString &name);
	QString importCircuitFromFile(const QString &filename);
	QString importVerilogModule(const QString &filename);
	QString createVerilogModuleFromText(const QString &text);

	CoreScene &getSceneOf(const QString &name);
	const lcsm::LCSMCircuit &getCircuitOf(const QString &name);
	const lcsm::verilog::Module &getVerilogOf(const QString &name);

	void removeCircuitOrVerilog(const QString &name);

	void save();

  private:
	QString m_name;
	QString m_filename;

	QHash< QString, lcsm::LCSMCircuit > m_circuits;
	QHash< QString, lcsm::verilog::Module > m_verilogs;
	std::map< QString, CoreScene > m_scenes;

	GUIOptions m_options;
};

#endif /* LCSM_VISUAL_CORE_PROJECT_H */
