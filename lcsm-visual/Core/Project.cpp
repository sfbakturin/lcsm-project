#include <Core/CoreScene.h>
#include <Core/Project.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/Verilog/Module.h>

#include <QHash>
#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <map>
#include <memory>

static constexpr int HEIGHT = 4096;
static constexpr int WIDTH = 4096;
static constexpr int GRID_SIZE = 32;

Project::Project(const QString &name) : m_name(name), m_options(HEIGHT, WIDTH, GRID_SIZE) {}

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

std::unique_ptr< Project > Project::fromFile(const QString &)
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

QString Project::createEmptyCircuit(const QString &n)
{
	m_circuits[n] = lcsm::LCSMCircuit(n.toStdString());
	m_scenes[n] = CoreScene(this, std::addressof(m_circuits[n]), std::addressof(m_options));
	return n;
}

QString Project::importCircuitFromFile(const QString &fn)
{
	std::string filename = fn.toStdString();
	lcsm::LCSMCircuit circuit = lcsm::LCSMCircuit::fromFile(filename);
	const QString name = QString::fromStdString(circuit.name());
	m_circuits[name] = std::move(circuit);
	m_scenes[name] = CoreScene(this, std::addressof(m_circuits[name]), std::addressof(m_options));
	return name;
}

QString Project::importVerilogModule(const QString &fn)
{
	std::string filename = fn.toStdString();
	lcsm::verilog::Module module = lcsm::verilog::Module::fromFile(filename);
	const QString name = QString::fromStdString(module.identifier());
	m_verilogs[name] = std::move(module);
	return name;
}

QString Project::createVerilogModuleFromText(const QString &s)
{
	lcsm::verilog::Module module = lcsm::verilog::Module::fromString(s.toStdString());
	const QString name = QString::fromStdString(module.identifier());
	m_verilogs[name] = std::move(module);
	return name;
}

CoreScene &Project::getSceneOf(const QString &name)
{
	return m_scenes[name];
}

const lcsm::LCSMCircuit &Project::getCircuitOf(const QString &name)
{
	return m_circuits[name];
}

const lcsm::verilog::Module &Project::getVerilogOf(const QString &name)
{
	return m_verilogs[name];
}

void Project::removeCircuitOrVerilog(const QString &name)
{
	if (isCircuitExists(name))
	{
		m_circuits.remove(name);
	}
	else if (isVerilogExists(name))
	{
		m_verilogs.remove(name);
	}
}

void Project::save() {}

void Project::exportToFile(const QString &name, const QString &fn)
{
	const std::string &filename = fn.toStdString();
	const lcsm::LCSMCircuit &circuit = getCircuitOf(name);
	circuit.dumpToFile(filename);
}
