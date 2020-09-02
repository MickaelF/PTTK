#include "aboutdialog.h"

#include <QDesktopServices>
#include <QUrl>

AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent)
{
    setupUi(this);
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    connect(g_close, &QPushButton::pressed, [&] { close(); });
    connect(g_github, &QPushButton::pressed, this, &AboutDialog::openGitHub);
    connect(g_website, &QPushButton::pressed, this, &AboutDialog::openWebsite);
}

void AboutDialog::openWebsite() const 
{
    QDesktopServices::openUrl(QUrl("http://mickael-fleurus.com"));
}

void AboutDialog::openGitHub() const
{
    QDesktopServices::openUrl(QUrl("https://github.com/MickaelF/PTTK"));
}
