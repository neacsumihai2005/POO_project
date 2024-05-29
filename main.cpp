#include <iostream>
#include <fstream>
#include <assert.h>
#include <exception>
#include <ctime>
#include <random>

#include "fitness.h"
#include "MOTD.h"
#include "coinflip.h"
#include "ColorManager.h"
#include "clothing.h"

using namespace workoutDiary;

inline void printMainScreenOptions(std::ostream & out){
    out << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << "\n";
    out << "~ 1 = start a new workout           ~" << "\n";
    out << "~ 2 = modify records                ~" << "\n";
    out << "~ 3 = print current personal PRs    ~" << "\n";
    out << "~ 4 = challenge another user        ~" << "\n";
    out << "~ 5 = randomly change color         ~" << "\n";
    out << "~ 6 = reset color to white          ~" << "\n";
    out << "~ 7 = view gym product prices       ~" << "\n";
    out << "~ other number = exit               ~" << "\n";
    out << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << "\n";
}
inline void printModifyScreenOptions(std::ostream & out){
    out << "------------------------------------" << "\n";
    out << "- 1 = delete a certain exercise    -" << "\n";
    out << "- 2 = modify a certain PR manually -" << "\n";
    out << "- other number = exit              -" << "\n";
    out << "------------------------------------" << "\n";
}
void printAllExercises(std::ostream & out, std::map<std::string, exerciseType*> const &exerciseMap){
    int iteratorNumber = 1;
    for(auto it : exerciseMap){
        out << iteratorNumber << ". " << it.first << "\n";
        iteratorNumber++;
    }
}

inline int addExercise(std::string const currentExerciseName, std::map<std::string, exerciseType*> &exerciseMap){
    ///handles the menu for the user when executing an exercise
    ///returns the PR for repetitions if there is any; 0 otherwise

    exercise * currentExercise = new exercise();
    exerciseType * currentExerciseType;

    ///this might be a new exercise, so we verify that
    if(exerciseMap.find(currentExerciseName) == exerciseMap.end()){
        currentExerciseType = new exerciseType(currentExerciseName);
        exerciseMap[currentExerciseName] = currentExerciseType;

        std::cout << "New exercise for you detected!" << "\n";
        std::cout << "Would you like to add a personal description? (y/n)" << "\n";

        char userChoice;
        std::cin >> userChoice;
        if( !(userChoice == 'n' || userChoice == 'y') ){
            throw(std::runtime_error("Invalid choice!"));
        }
        if(userChoice == 'y'){
            std::cout << "Description (one liner) = ";
            std::string newDescription;
            std::cin.get();
            std::getline(std::cin, newDescription);
            currentExerciseType -> setDescription(newDescription);
        }

    }
    else {
        currentExerciseType = exerciseMap[currentExerciseName];
    }

    std::cout << "Weight (kg) = ";
    int weight;
    std::cin >> weight;
    assert(weight > 0);
    currentExercise -> setWeight(weight);

    std::cout << "Reps = ";
    int reps;
    std::cin >> reps;
    int finalPR = 0;
    while(reps != 0){
        currentExercise -> addSet(reps);

        int previousPR = currentExerciseType -> getPR(weight);
        currentExerciseType -> tryUpdatePR(weight, reps);
        int potentialPR = currentExerciseType -> getPR(weight);

        if(potentialPR > previousPR){
            std::cout << "Congratulations! You just hit a PR! Old PR was " << previousPR << " reps!" << "\n";
            finalPR = potentialPR;
        }

        std::cout << "Reps = ";
        std::cin >> reps;
        assert(reps >= 0);
    }

    return finalPR;
}

inline void menuNewWorkout(std::istream& in, std::map<std::string, exerciseType*> &exerciseMap){
    calendaristicDate currentDate;
    std::cin >> currentDate;

    try{
        if( currentDate.validDate() ){
            std::cout << "!!Type \"stop\" in the Exercise box in order to end the workout!!" << "\n";
            workout* temp = new workout(currentDate);

            std::cout << "Exercise = ";
            std::string currentExerciseName;
            std::cin >> currentExerciseName;

            while(currentExerciseName != "stop"){
                addExercise(currentExerciseName, exerciseMap);

                std::cout << "Exercise = ";
                std::cin >> currentExerciseName;
            }

            std::cout << "Workout ended! Congratulations!" << "\n" << "\n";
        }
        else {
            exceptionDate mce;
            throw(mce);
        }
    }
    catch(exceptionDate mce){
        std::cout << mce.what() << "\n";
        return;
    }

}

void getClientsList(std::istream &in, std::vector<Client> &clients){
    clients.clear();
    Client newClient;
    while(in >> newClient){
        Person::incNumberOfPersons();
        clients.push_back(newClient);
    }
}

void getExerciseMap(std::istream &in, std::map< std::string, std::map<std::string, exerciseType*> > &exerciseMap){
    std::string currentUsername;
    while(in >> currentUsername){
        int numberOfUserExercises;
        in >> numberOfUserExercises;
        for(int itr = 0; itr < numberOfUserExercises; itr++){
            std::string currentExerciseName;
            in >> currentExerciseName;
            //exerciseType::incrementNumberOfExercises();
            exerciseType * newEntry = new exerciseType(currentExerciseName);

            int numberOfRecords;
            in >> numberOfRecords;
            for(int i = 0; i < numberOfRecords; i++){
                int weight;
                in >> weight;

                int reps;
                in >> reps;

                newEntry -> tryUpdatePR(weight, reps);
            }


            ///we built newEntry; now we add it to the exerciseMap
            exerciseMap[currentUsername][currentExerciseName] = newEntry;
        }
    }
}

void outputClientsLogs(std::ostream& out, std::vector<Client> clients){
    for(int i = 0; i < clients.size(); i++){
        clients[i].outputCredentials(out); out << "\n";
    }
}

void outputClientsList(std::ostream& out, std::vector<Client> clients){
    for(int i = 0; i < clients.size(); i++){
        out << (i + 1) << ". " << clients[i] << "\n";
    }
}

void outputExerciseMap(std::ostream& out, std::map<std::string, std::map<std::string, exerciseType*> > const & exerciseMap){
    for (auto itName : exerciseMap){
        out << itName.first << "\n";
        out << itName.second.size() << "\n";
        for(auto itExercise : itName.second){
            out << (itExercise.second) -> getName() << "\n";
            (itExercise.second) -> printRecords(out, false);
        }
    }
}

int getRandomNumber(int st, int dr) {
    static std::random_device rd;  // Declare random device as static
    static std::mt19937 eng(rd()); // Declare Mersenne Twister engine as static
    static std::uniform_int_distribution<> distr(st, dr); // Declare distribution as static

    return distr(eng); // Generate and return a random number
}
void loadingBar()
{
    // Initialize char for printing
    // loading bar
    char a = 177, b = 219;

    std::cout << "\n\n\n\t\t\t\t\t";
    std::cout << "Loading...\n\n";
    std::cout << "\t\t\t\t\t";

    // Print initial loading bar
    for (int i = 0; i < 26; i++)
        std::cout << a;

    // Set the cursor again starting
    // point of loading bar
    std::cout << "\r";
    std::cout << "\t\t\t\t\t";

    // Print loading bar progress
    for (int i = 0; i < 26; i++) {
        std::cout << b;

        // Sleep for 50 ms
        Sleep(50);
    }
    std::cout << "\n\n\n\n";
}

int main()
{
    // Initializing static member instance to nullptr
    ConsoleColorManager* colorManager = ConsoleColorManager::getInstance();

    const std::time_t now = std::time(nullptr);
    const std::tm calendar_time = *std::localtime( std::addressof(now) );

    ///upcasting and dynamic dispatch:
    MOTDProvider * provider; ///base class pointer
    if(calendar_time.tm_hour <= 12){
        ///morning MOTD
        provider = new MorningMOTD();
    }
    else {
        ///evening MOTD
        provider = new EveningMOTD();
    }
    std::cout << provider -> getIntro() << "\n";
    std::cout << provider -> getMOTD() << "\n" << "\n";

    /*
    std::ifstream finManagers("managers.txt");
    std::string managerName;
    std::string managerEmail;
    while(finManagers >> managerName){
        finManagers >> managerEmail;
        //Manager* tmp = new Manager();
        //tmp -> display(cout);
        std::cout << "\n";

        //delete tmp;
    }
    finManagers.close();
    */


    std::ifstream finClients("clients.txt");
    std::vector<Client> clients;
    getClientsList(finClients, clients);
    finClients.close();

    std::cout << "List of clients:" << "\n";
    outputClientsList(std::cout, clients);

    bool newUserFlag = false;
    std::cout << "username = ";
    std::string username;
    std::cin >> username;

    int clientID = -1;
    for(int i = 0; i < clients.size(); i++){
        if(clients[i].getName() == username){
            clientID = i;
            break;
        }
    }

    if(clientID == -1){
        std::cout << "Would you like to register? (y/n)" << "\n";
        char userChoice;
        std::cin >> userChoice;

        if(!(userChoice == 'y' || userChoice == 'n')){
            std::cout << "Invalid choice!" << "\n";
            return -1;
        }

        if(userChoice == 'n'){
            std::cout << "Program terminated.";
            return 0;
        }
        else {
            newUserFlag = true;

            Client newClient(username);
            std::cout << "new password = ";
            std::string password;
            std::cin >> password;
            newClient.setPassword(password);
            clients.push_back(newClient);
            clientID = clients.size() - 1;

            std::ofstream foutClientsAppend ("clients.txt", std::ios::app);
            foutClientsAppend << username << ' ' << password << "\n";
            foutClientsAppend.close();

            std::ofstream foutExercisesAppend ("exercises.txt", std::ios::app);
            foutExercisesAppend << username << "\n";
            foutExercisesAppend << 0 << "\n";
            foutExercisesAppend.close();
        }
    }
    else {
        std::cout << "password = ";
        std::string password;
        std::cin >> password;

        if(password != clients[clientID].getPassword()){
            std::cout << "Invalid Password!\n";
            return -1;
        }

    }
    loadingBar();

    //std::cout << Client::returnTestID();
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << "\n";
    std::cout << "!!Disclaimer: please use the singular form for each exercise         !!" << "\n";
    std::cout << "!!For example: \"dip\" and \"dips\" are registered as different exercises!!" << "\n";
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << "\n";
    std::cout << "\n";

    if(newUserFlag) {
        std::cout << "~~Welcome, ";
    }
    else {
        std::cout << "~~Welcome back, ";
    }
    std::cout << clients[clientID].getName() << "!" << "~~" << "\n";

    int userCurrentChoice = 0;
    ///cute little menu implementation
    std::ifstream finExercises ("exercises.txt");
    ///updateInput(fin);

    std::map<std::string, std::map<std::string, exerciseType*> > exerciseMap;
    for (auto it : clients){
        exerciseMap[ it.getName() ]; ///initialise exerciseMap even on those clients who haven't done any exercises yet
    }
    getExerciseMap(finExercises, exerciseMap);

    ///std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << "\n";
    std::cout << "Here are your current lifetime records:" << "\n";
    if(exerciseMap[ clients[clientID].getName() ].empty()){
        std::cout << "No logs detected yet." << "\n";
    }
    for (auto it : exerciseMap[ clients[clientID].getName() ]){
        ///std::cout << it.first << "!";
        it.second -> printRecords(std::cout, true);
        ///std::cout << "\n";
    }
    ///std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << "\n";
    std::cout << "\n";


    do{
        printMainScreenOptions(std::cout);
        std::cin >> userCurrentChoice;

        if( !(1 <= userCurrentChoice && userCurrentChoice <= 7) ){
            std::cout << "Program terminated!" << "\n";
            break;
        }

        if(userCurrentChoice == 1){
            ///start a new workout
            menuNewWorkout(std::cin, exerciseMap[ clients[clientID].getName() ]);
        }
        else if(userCurrentChoice == 2){
            ///modify records
            printModifyScreenOptions(std::cout);
            std::cin >> userCurrentChoice;

            if(userCurrentChoice == 1){
                if(exerciseMap[ clients[clientID].getName() ].size() == 0){
                    std::cout << "No exercises to delete!" << "\n";

                    continue;
                }
                ///delete an exercise altogether
                printAllExercises(std::cout, exerciseMap[ clients[clientID].getName() ]);
                std::cout << "Which exercise do you want to delete? (" << 1 << " to " << exerciseMap[ clients[clientID].getName() ].size() << ")" << "\n";

                int indexToDelete;
                std::cout << "Number = ";
                std::cin >> indexToDelete;
                if( ! (1 <= indexToDelete && indexToDelete <= exerciseMap[ clients[clientID].getName() ].size()) ){
                    throw(std::runtime_error("Invalid number!"));
                }


                auto iteratorToDelete = exerciseMap[ clients[clientID].getName() ].begin();
                for(int times = 1; times <= indexToDelete - 1; times++){
                    iteratorToDelete++;
                }
                std::cout << "***" << iteratorToDelete -> first << " deleted succesfully!***" << "\n" << "\n";
                delete iteratorToDelete -> second; ///free up memory
                exerciseMap[ clients[clientID].getName() ].erase(iteratorToDelete);


            }
            else if(userCurrentChoice == 2){
                ///modify a certain PR manually
                if(exerciseMap[ clients[clientID].getName() ].size() == 0){
                    std::cout << "No exercises to modify!" << "\n";

                    continue;
                }

                printAllExercises(std::cout, exerciseMap[ clients[clientID].getName()]);
                std::cout << "Which exercise do you want to update? (" << 1 << " to " << exerciseMap[ clients[clientID].getName() ].size() << ")" << "\n";

                int indexToUpdate;
                std::cout << "Number = ";
                std::cin >> indexToUpdate;
                if( ! (1 <= indexToUpdate && indexToUpdate <= exerciseMap[ clients[clientID].getName() ].size()) ){
                    throw(std::runtime_error("Invalid number!"));
                }


                auto iteratorToUpdate = exerciseMap[ clients[clientID].getName() ].begin();
                for(int times = 1; times <= indexToUpdate - 1; times++){
                    iteratorToUpdate++;
                }

                std::cout << "Weight to update (kg) = ";
                int weightToUpdate;
                std::cin >> weightToUpdate;
                if( ! (weightToUpdate > 0) ){
                    throw(std::runtime_error("Negative weight!"));
                }

                std::cout << "New PR for reps = ";
                int repsToUpdate;
                std::cin >> repsToUpdate;
                if( ! (repsToUpdate > 0) ){
                    throw(std::runtime_error("Negative reps!"));
                }

                (iteratorToUpdate -> second) -> forceUpdatePR(weightToUpdate, repsToUpdate);
            }
        }
        else if(userCurrentChoice == 3){
            ///show current global PRs
            if(exerciseMap[ clients[clientID].getName() ].size() == 0){
                std::cout << "No logs detected yet!" << "\n";
            }
            else for (auto it : exerciseMap[ clients[clientID].getName() ]){
                ///std::cout << it.first << "!";
                it.second -> printRecords(std::cout, true);
                ///std::cout << "\n";
            }
        }
        else if(userCurrentChoice == 4){
            if(clients.size() == 1){ ///at all times there is at least one client; but if it's the only client, he has no one to challenge :(
                std::cout << "No clients to challenge besides you!" << "\n";
                continue;
            }

            for(int i = 0; i < clients.size(); i++){
                std::cout << (i + 1) << ". " << clients[i].getName() << "\n";
            }
            std::cout << "Which user would you like to challenge? (1 to " << clients.size() << ")" << "\n";
            int indexToChallenge;
            std::cout << "Number = ";
            std::cin >> indexToChallenge;
            while( ! (1 <= indexToChallenge && indexToChallenge <= clients.size()) || indexToChallenge == clientID + 1){
                if( ! (1 <= indexToChallenge && indexToChallenge <= clients.size()) ){
                    std::cout << "Invalid number! Try again." << "\n";
                }
                else {
                    std::cout << "Can't challenge yourself! Try again." << "\n";
                }
                std::cout << "Number = ";
                std::cin >> indexToChallenge;
            }

            std::cout << "User " << clients[indexToChallenge - 1].getName() << ", please enter your password:\npassword = ";
            std::string challengedPassword;
            std::cin >> challengedPassword;

            if(challengedPassword != clients[indexToChallenge - 1].getPassword()){
                std::cout << "Invalid Password!" << "\n";
                continue;
            }


            std::cout << "What challenge do you want to partake in?" << "\n";
            std::cout << "1. Workout challenge!" << "\n";
            std::cout << "2. (fun) Coin flip!" << "\n";

            int indexChallenge;
            std::cin >> indexChallenge;
            while( ! (1 <= indexChallenge && indexChallenge <= 2) ){
                std::cout << "Invalid number! Try again. " << "\n";
                std::cin >> indexChallenge;
            }

            if(indexChallenge == 1){
                std::cout << "Exercise = ";
                std::string currentExerciseName;
                std::cin >> currentExerciseName;
                std::cout << "Rules:" << "\n";
                std::cout << "*If someone hits a new rep PR, the person wins" << "\n";
                std::cout << "*If both hit a PR, or none hit a PR, it's a draw" << "\n";

                std::cout << clients[clientID].getName() << "'s turn" << "\n";
                int resultPlayerOne = addExercise(currentExerciseName, exerciseMap[ clients[clientID].getName() ]);

                std::cout << clients[indexToChallenge - 1].getName() << "'s turn" << "\n";
                int resultPlayerTwo = addExercise(currentExerciseName, exerciseMap[ clients[indexToChallenge - 1].getName() ]);


                if((resultPlayerOne != 0 && resultPlayerTwo != 0) || (resultPlayerOne == 0 && resultPlayerTwo == 0)){
                    std::cout << "Congratulations! It is a draw" << "\n";
                }
                else {
                    if(resultPlayerOne != 0){
                        std::cout << clients[clientID].getName() << " wins! Better luck next time, " << clients[indexToChallenge - 1].getName() << " :(" << "\n";
                    }
                    else {
                        std::cout << clients[indexToChallenge - 1].getName() << " wins! Better luck next time, " << clients[clientID].getName() << " :( " << "\n";
                    }
                }
            }
            else if(indexChallenge == 2){
                ///Coin Flip

                Coin coin;
                Player player1("Player " + clients[clientID].getName());
                Player player2("Player " + clients[indexToChallenge - 1].getName());

                std::shared_ptr<ChatConnection> playerOneConnection = std::make_shared<ChatConnection> (clients[clientID].getName(),
                                                                                                        clients[indexToChallenge - 1].getName());
                std::shared_ptr<ChatConnection> playerTwoConnection = playerOneConnection;

                std::cout << clients[clientID].getName() << ", send a message for " << clients[indexToChallenge - 1].getName() << " (max 50 char)" << "\n";
                std::string chatmessage;
                std::cin.get();
                std::getline(std::cin, chatmessage);
                playerOneConnection -> sendMessage(clients[clientID].getName(), chatmessage);

                std::cout << clients[indexToChallenge - 1].getName() << ", send a message for " << clients[clientID].getName() << " (max 50 char)" << "\n";
                std::getline(std::cin, chatmessage);
                playerTwoConnection -> sendMessage(clients[indexToChallenge - 1].getName(), chatmessage);

                coin.attach(&player1);
                coin.attach(&player2);

                std::cout << "Let's flip the coin! Press y to continue" << "\n";
                char userAccept;
                std::cin >> userAccept;
                if(userAccept != 'y'){
                    std::cout << "Not this time!" << "\n";
                    continue;
                }

                coin.flip();

                coin.detach(&player2);
                std::cout << "Removing " << clients[indexToChallenge - 1].getName() << " from observers!" << "\n";
                loadingBar();
                std::cout << "Let's flip the coin again! Press y to continue" << "\n";
                std::cin >> userAccept;
                if(userAccept != 'y'){
                    std::cout << "Not this time!" << "\n";
                    continue;
                }

                coin.flip();
            }


            ///verify if this exercise exists for players 1 and 2
            ///and if not; create it for each individual where it may be the case
        }
        else if(userCurrentChoice == 5){
            colorManager->setConsoleColor( getRandomNumber(2, 14) ); // Generate and return a random number
        }
        else if(userCurrentChoice == 6){
            colorManager->setConsoleColor(7);
        }
        else if(userCurrentChoice == 7){
            std::cout << "Enter the article you want to calculate the price of (Tshirt, Shorts, Shoes)" << "\n";
            std::cout << "Article = ";
            std::string clothType;
            std::cin >> clothType;
            std::transform(clothType.begin(), clothType.end(), clothType.begin(),
                // static_cast<int(*)(int)>(std::toupper)         // wrong
                // [](int c){ return std::toupper(c); }           // wrong
                // [](char c){ return std::toupper(c); }          // wrong
                   [](unsigned char c){ return std::tolower(c); } // correct
                  );

            std::unique_ptr<Cloth> cloth = ClothFactory::createCloth(clothType);
            if(cloth == nullptr){
                std::cout << "Invalid article type!" << "\n";
                continue;
            }
            std::cout << "Please enter a size (XS, S, M, L, XL, XXL)" << "\n";
            std::cout << "Size = ";
            std::string userSize;
            std::cin >> userSize;
            std::transform(userSize.begin(), userSize.end(), userSize.begin(),
                // static_cast<int(*)(int)>(std::toupper)         // wrong
                // [](int c){ return std::toupper(c); }           // wrong
                // [](char c){ return std::toupper(c); }          // wrong
                   [](unsigned char c){ return std::toupper(c); } // correct
                  );
            if(!(
                 userSize == "XS" ||
                 userSize == "S" ||
                 userSize == "M" ||
                 userSize == "L" ||
                 userSize == "XL" ||
                 userSize == "XXL"
                 )){
                    std::cout << "Invalid size!" << "\n";
                    continue;
            }


            std::cout << "The price is: " << cloth->getPrice(userSize) << " lei" << "\n";


        }
    }while( 1 <= userCurrentChoice && userCurrentChoice <= 7 );

    finExercises.close();

    std::ofstream foutExercises ("exercises.txt");
    outputExerciseMap(foutExercises, exerciseMap);
    foutExercises.close();


    std::ofstream foutClients("clients.txt");
    outputClientsLogs(foutClients, clients);
    foutClients.close();
    return 0;
}
