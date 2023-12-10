#ifndef SOCIALNETWORKWINDOW_H
#define SOCIALNETWORKWINDOW_H

#include "network.h"
#include <QMainWindow>
#include <QTableWidget>
#include <stack>

QT_BEGIN_NAMESPACE
namespace Ui { class SocialNetworkWindow; }
QT_END_NAMESPACE

class SocialNetworkWindow : public QMainWindow
{
    Q_OBJECT

public:
    SocialNetworkWindow(QWidget *parent = nullptr);
    ~SocialNetworkWindow();

    void makeTablesPretty();

    void displayLoginScreen();

    void displayProfile();

private slots:
    void on_loginButton_clicked();

    void on_currUserFriendsTable_itemClicked(QTableWidgetItem *item);

    void on_uliBackButton_clicked();

    void on_uliFriendSuggestions_itemClicked(QTableWidgetItem *item);

    void on_addFriendButton_clicked();

    void on_universalBackButton_clicked();

private:
    Ui::SocialNetworkWindow *ui;
    Network jitter;
    User * userLoggedIn;
    User * userDisplayed;
    std::stack<int> uiHistory;
};
#endif // SOCIALNETWORKWINDOW_H
