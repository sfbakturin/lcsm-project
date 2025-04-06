#include <Core/Project.h>
#include <Support/Strings.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/Verilog/Module.h>

#include <QHash>
#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <memory>

Project::Project(const QString &name) : m_name(name) {}

const QString &Project::name() const noexcept
{
	return m_name;
}

const QString &Project::filename() const noexcept
{
	return m_filename;
}

void Project::setFilename(const QString &filename)
{
	m_filename = filename;
}

bool Project::isSavePointDetermined() const noexcept
{
	return !filename().isEmpty();
}

std::unique_ptr< Project > Project::fromFile(const QString &filename)
{
	return nullptr;
}

bool Project::isCircuitExists(const QString &name) const
{
	return m_circuits.contains(name);
}

bool Project::isVerilogExists(const QString &name) const
{
	return m_verilogs.contains(name);
}

void Project::createEmptyCircuit(const QString &n)
{
	// std::string name = n.toStdString();
	// m_circuits[n] = lcsm::LCSMCircuit(std::move(name));
	m_circuits[n] = lcsm::LCSMCircuit(QtStringToAsciiStd(n));
}

void Project::importCircuitFromFile(const QString &fn)
{
	std::string filename = fn.toStdString();
	lcsm::LCSMCircuit circuit = lcsm::LCSMCircuit::fromFile(filename);
	const QString name = QString::fromStdString(circuit.name());
	m_circuits[name] = std::move(circuit);
}

void Project::importVerilogModule(const QString &fn)
{
	std::string filename = fn.toStdString();
	lcsm::verilog::Module module = lcsm::verilog::Module::fromFile(filename);
	const QString name = QString::fromStdString(module.identifier());
	m_verilogs[name] = std::move(module);
}

void Project::save() {}
