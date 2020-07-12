#include "alertdialog.h"

namespace
{
constexpr char* levelToString(AlertDialog::Level l)
{
    switch (l)
    {
        case AlertDialog::Level::Info: return "Info";
        case AlertDialog::Level::Warning: return "Warning";
        case AlertDialog::Level::Error: return "Error";
        case AlertDialog::Level::Fatal: return "Fatal";
        default: return "unknown";
    }
}
constexpr char* levelIcon(AlertDialog::Level l)
{
    switch (l)
    {
        case AlertDialog::Level::Info: return ":/resources/info.png";
        case AlertDialog::Level::Warning: return ":/resources/warning.png";
        case AlertDialog::Level::Error: return ":/resources/error.png";
        case AlertDialog::Level::Fatal: return ":/resources/fatal.png";
        default: return ":/resources/empty.png";
    }
}
} // namespace

AlertDialog::AlertDialog(AlertDialog::Level l, AlertDialog::Option o,
                         const QString& message,
                         const QString& title, QWidget* parent)
    : QDialog(parent)
{
    setupUi(this);
    setWindowTitle(title.isEmpty() ? levelToString(l) : title);
    g_text->setText(message);
    g_icon->setPixmap(QPixmap(levelIcon(l)).scaledToHeight(32));

    if (o == AlertDialog::Option::Ok)
    {
        g_spacer->changeSize(0, 0, QSizePolicy::Ignored);
        g_cancel->setVisible(false);
    }
    connect(g_ok, &QPushButton::pressed, this, [&]() { return accept(); });
    connect(g_cancel, &QPushButton::pressed, this, [&]() { return rejected(); });
    exec();
}
