#include "aicreatorplugin.h"
#include <coreplugin/icore.h>
#include <coreplugin/locator/locatormanager.h>
#include <texteditor/texteditor.h>
#include <QProcess>
#include <QMessageBox>

namespace AICreator {
namespace Internal {

AICreatorPlugin::AICreatorPlugin() {}
AICreatorPlugin::~AICreatorPlugin() {}

bool AICreatorPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorString);

    auto filter = new AILocatorFilter();
    addAutoReleasedObject(filter);

    return true;
}

void AICreatorPlugin::extensionsInitialized() {}

ExtensionSystem::IPlugin::ShutdownFlag AICreatorPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}

AILocatorFilter::AILocatorFilter()
{
    setId("AICommandFilter");
    setDisplayName(tr("AI Command Assistant"));
    setShortcutString(QStringLiteral("ai"));
    setPriority(High);
}

QList<Core::LocatorFilterEntry> AILocatorFilter::matchesFor(
    Core::QmlLocatorFilter *interest, const QString &entry)
{
    Q_UNUSED(interest);
    QList<Core::LocatorFilterEntry> results;
    if (entry.isEmpty())
        return results;

    Core::LocatorFilterEntry result(this, QString("Ask AI: %1").arg(entry), QVariant(entry));
    results.append(result);
    return results;
}

void AILocatorFilter::accept(Core::LocatorFilterEntry selection, 
                            QString *newText, 
                            int *selectionStart, 
                            int *selectionLength) const
{
    Q_UNUSED(newText);
    Q_UNUSED(selectionStart);
    Q_UNUSED(selectionLength);

    QString prompt = selection.internalData.toString();
    QString scriptPath = QDir::currentPath() + "/ai-core.py";
    
    QProcess *process = new QProcess();
    process->setProgram("python3");
    process->setArguments({scriptPath, prompt});
    
    QObject::connect(process, &QProcess::finished, [process]() {
        QString output = process->readAllStandardOutput();
        
        auto editor = TextEditor::BaseTextEditor::currentTextEditor();
        if (editor) {
            editor->insert(output);
        } else {
            QMessageBox::information(Core::ICore::dialogParent(), "AI Generator Output", output);
        }
        process->deleteLater();
    });
    
    process->start();
}

} // namespace Internal
} // namespace AICreator