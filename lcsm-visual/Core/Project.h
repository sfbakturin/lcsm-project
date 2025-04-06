#ifndef LCSM_VISUAL_CORE_PROJECT_H
#define LCSM_VISUAL_CORE_PROJECT_H

#include <Core/Scene.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/Verilog/Module.h>

#include <QDateTime>
#include <QHash>
#include <QString>
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

	void createEmptyCircuit(const QString &name);
	void importCircuitFromFile(const QString &filename);
	void importVerilogModule(const QString &filename);

	void save();

  private:
	QString m_name;
	QString m_filename;

	QHash< QString, lcsm::LCSMCircuit > m_circuits;
	QHash< QString, lcsm::verilog::Module > m_verilogs;
	QHash< QString, Scene > m_scenes;
};

#endif /* LCSM_VISUAL_CORE_PROJECT_H */
