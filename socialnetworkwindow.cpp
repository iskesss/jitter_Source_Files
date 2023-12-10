#include "socialnetworkwindow.h"
#include "ui_socialnetworkwindow.h"
#include <QFile>

SocialNetworkWindow::SocialNetworkWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SocialNetworkWindow)
{
    ui->setupUi(this);
    jitter.readUsers("social_network.txt");
    jitter.readPosts("posts.txt");
    makeTablesPretty(); // not integral to functionality whatsoever
    displayLoginScreen();
}

SocialNetworkWindow::~SocialNetworkWindow()
{
    delete ui;
}

// makeTablesPretty function
// Pre: None
// Post: Literally just makes the 2 UI tables aesthetically pleasing.
void SocialNetworkWindow::makeTablesPretty(){ // purely intended to make the QTables aesthetically pleasing (found this stuff online)
    ui->uliFriendSuggestions->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->uliFriendSuggestions->verticalHeader()->setHidden(true);
    ui->uliFriendSuggestions->horizontalHeader()->setHidden(true);

    ui->currUserFriendsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->currUserFriendsTable->verticalHeader()->setHidden(true);
    ui->currUserFriendsTable->horizontalHeader()->setHidden(true);
}

void SocialNetworkWindow::displayLoginScreen(){
    ui->loginButton->show();
    ui->loginField->show();
    ui->loginTitle->show();
    ui->friendsListTitle->hide();
    ui->topFriendSuggestionsTitle->hide();
    ui->recentPostsTitle->hide();
    ui->clickToInstaFriendTitle->hide();
    ui->currProfileName->hide();
    ui->currUserFriendsTable->hide();
    ui->uliFriendSuggestions->hide();
    ui->currProfilePosts->hide();
    ui->uliBackButton->hide();
    ui->addFriendButton->hide();
    ui->universalBackButton->hide();
}

// displayProfile function
// Pre: The on_loginButton_clicked function has been called.
// Post: Displays all posts (private posts included), the friends list, and suggested friends list for the logged in user. For any other user this function displays their public posts and friends list, as well as the addFriendButton and the uliBackButton.
void SocialNetworkWindow::displayProfile(){
    if(uiHistory.top() == -1){
        ui->universalBackButton->setText( "<- Back to Login" );
    }
    else if(uiHistory.top() == 0){
        ui->universalBackButton->setText( "<- Back to My Profile" );
    }
    else{ // we fetch the user at the top of the stack and display their first name (which we extract from their full name) on the universalBackButton
        size_t i = 0;
        std::string wholeName = jitter.getUser(uiHistory.top())->getName();
        std::string firstName = "";

        while( wholeName[i] != ' '){
            firstName += wholeName[i];
            ++i;
        }
        ui->universalBackButton->setText( "<- Back to " + QString::fromStdString(firstName) + "'s Profile" );
    }
    if(userDisplayed == userLoggedIn){
        ui->currProfileName->setText("My Profile");
        //ui->uliBackButton->hide();
        ui->addFriendButton->hide();
        ui->topFriendSuggestionsTitle->show();
        ui->clickToInstaFriendTitle->show();
        ui->uliFriendSuggestions->show();
        std::size_t allTimeHighestScore;
        std::vector<std::size_t> suggestedFriendsVec = jitter.suggestFriends(userLoggedIn->getID(),allTimeHighestScore);

        ui->uliFriendSuggestions->setRowCount(suggestedFriendsVec.size());
        for(std::size_t i = 0; i < suggestedFriendsVec.size(); ++i){
            ui->uliFriendSuggestions->setItem(i,0,new QTableWidgetItem(QString::fromStdString( jitter.getUser(suggestedFriendsVec[i])->getName() )));
        }
    }
    else{
        ui->currProfileName->setText("Now viewing " + QString::fromStdString(userDisplayed->getName()) + "'s profile");
        ui->uliBackButton->show();
        ui->addFriendButton->show();
        ui->topFriendSuggestionsTitle->hide();
        ui->clickToInstaFriendTitle->hide();
        ui->uliFriendSuggestions->hide();
    }

    std::set<std::size_t> friendsList = userDisplayed->getFriends();

    std::size_t counter = 0;
    ui->currUserFriendsTable->setRowCount(friendsList.size());

    for(auto currFriend : friendsList){
        ui->currUserFriendsTable->setItem(counter,0, new QTableWidgetItem(QString::fromStdString(jitter.getUser(currFriend)->getName())));
        ++counter;
    }

    ui->currProfilePosts->setText(QString::fromStdString( userDisplayed->displayPosts(5,userDisplayed != userLoggedIn) ));

}

// on_loginButton_clicked function
// Pre: There exists a user on the network whose name we know.
// Post: The login screen UI has been hidden and the profile of the logged in user is displayed.
void SocialNetworkWindow::on_loginButton_clicked()
{
    QString givenName = ui->loginField->toPlainText();
    if(jitter.getID(givenName.toStdString()) != -1){
        ui->loginButton->hide();
        ui->loginField->hide();
        ui->loginTitle->hide();
        ui->currProfileName->show();
        ui->currUserFriendsTable->show();
        ui->currProfilePosts->show();
        ui->friendsListTitle->show();
        ui->topFriendSuggestionsTitle->show();
        ui->recentPostsTitle->show();
        ui->clickToInstaFriendTitle->show();
        ui->universalBackButton->show();
        ui->uliBackButton->show();
        userLoggedIn = userDisplayed = jitter.getUser( jitter.getID( givenName.toStdString() ) );
        uiHistory.push(-1); // -1 tells our program that the most recent UI was the login screen. (uiHistory.top() == -1) will mean we've reached the bottom of the uiHistory stack.
        displayProfile();
    }
    else{
        ui->loginTitle->setText("User not found, try again!");
    }
}

// on_currUserFriendsTable_itemClicked function
// Pre: The logged in user has at least one friend.
// Post: The profile of the clicked username is displayed.
void SocialNetworkWindow::on_currUserFriendsTable_itemClicked(QTableWidgetItem *item)
{
    uiHistory.push(userDisplayed->getID()); // this stores the ID of the user whose profile we were viewing BEFORE they clicked to another user's page. note that userDisplayed has not been updated quite yet!
    userDisplayed = jitter.getUser ( jitter.getID( item->text().toStdString() ) ); // and NOW we update userDisplayed.
    displayProfile();
}

// on_uliBackButton_clicked function
// Pre: None.
//Post: The profile of the logged in user is displayed again.
void SocialNetworkWindow::on_uliBackButton_clicked()
{
    if(userLoggedIn != userDisplayed){
        uiHistory.push(userDisplayed->getID());
        userDisplayed = userLoggedIn;
        displayProfile();
    }
    else{
        std::cout << "You're already viewing your profile." << std::endl;
    }
}


// on_uliFriendSuggestions_itemClicked function
// Pre: userLoggedIn has connections who qualify as suggestable
// Post: The suggested user and the logged in user in have been connected as friends, and the UI has refreshed.
void SocialNetworkWindow::on_uliFriendSuggestions_itemClicked(QTableWidgetItem *item)
{
    jitter.addConnection(userLoggedIn->getName(),item->text().toStdString());
    displayProfile(); // Refreshes the entire UI to reflect friending changes. Network::suggestFriends is run again, this time taking into account the friend that was just added. Top friend suggestions may instantly make intelligent changes based on this re-assessment.
}

// on_addFriendButton_clicked function
// Pre: There exists more than one user of the network.
// Post: The user currently being displayed and the logged in user have been connected as friends, and the UI has refreshed.
void SocialNetworkWindow::on_addFriendButton_clicked()
{
    jitter.addConnection(userLoggedIn->getName(),userDisplayed->getName());
    displayProfile(); // Refreshes the entire UI to reflect friending changes.
}


void SocialNetworkWindow::on_universalBackButton_clicked()
{
    if(uiHistory.top() >= 0){
        userDisplayed = jitter.getUser(uiHistory.top());
        uiHistory.pop();
        displayProfile();
    }
    else{
        displayLoginScreen();
    }
}

