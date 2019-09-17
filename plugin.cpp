#include "plugin.h"

#include "minimapproxystyle.h"
#include "settings.h"

#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/editormanager/ieditor.h>
#include <texteditor/texteditor.h>

#include <QApplication>

namespace Minimap::Internal {

MinimapPlugin::MinimapPlugin() noexcept {}

bool MinimapPlugin::initialize([[maybe_unused]] const QStringList &arguments,
                               [[maybe_unused]] QString *errorMessage) {
    // Qt parent-child relationship will cover deletion
    new Settings(this);

    auto *minimapStyle = new MinimapProxyStyle(QApplication::style());
    QApplication::setStyle(minimapStyle);
    QApplication::style()->moveToThread(QApplication::instance()->thread());

    auto *editorManager = Core::EditorManager::instance();
    QObject::connect(editorManager,
                     &Core::EditorManager::editorCreated,
                     this,
                     &MinimapPlugin::editorCreated);

    return true;
}

void MinimapPlugin::editorCreated(
    Core::IEditor *editor, [[maybe_unused]] const QString &fileName) const
    noexcept {
    auto baseEditor = qobject_cast<TextEditor::BaseTextEditor *>(editor);
    if (baseEditor != nullptr) {
        MinimapProxyStyle::baseEditorCreated(baseEditor);
    }
}

} // namespace Minimap::Internal
