#pragma once

#include <extensionsystem/iplugin.h>
#include <coreplugin/locator/ilocatorfilter.h>

namespace AICreator {
namespace Internal {

class AICreatorPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "AICreator.json")

public:
    AICreatorPlugin();
    ~AICreatorPlugin() override;

    bool initialize(const QStringList &arguments, QString *errorString) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;
};

class AILocatorFilter : public Core::ILocatorFilter
{
    Q_OBJECT
public:
    AILocatorFilter();
    
    QList<Core::LocatorFilterEntry> matchesFor(
        Core::QmlLocatorFilter *interest,
        const QString &entry) override;
        
    void accept(Core::LocatorFilterEntry selection, 
                QString *newText, 
                int *selectionStart, 
                int *selectionLength) const override;
    
    void refresh(QFutureInterface<void> &future) override {}
};

} // namespace Internal
} // namespace AICreator