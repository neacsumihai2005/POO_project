#include <iostream>
#include <fstream>
#include <assert.h>
#include <exception>
#include <ctime>

#include "fitness.h"
#include "MOTD.h"

using namespace workoutDiary;


inline void printMainScreenOptions(std::ostream & out){
    out << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << "\n";
    out << "~ 1 = start a new workout           ~" << "\n";
    out << "~ 2 = modify records                ~" << "\n";
    out << "~ 3 = print current personal PRs    ~" << "\n";
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
                assert(reps > 0);
                while(reps != 0){
                    currentExercise -> addSet(reps);

                    int previousPR = currentExerciseType -> getPR(weight);
                    currentExerciseType -> tryUpdatePR(weight, reps);
                    int potentialPR = currentExerciseType -> getPR(weight);

                    if(potentialPR > previousPR){
                        std::cout << "Congratulations! You just hit a PR! Old PR was " << previousPR << " reps!" << "\n";
                    }

                    std::cout << "Reps = ";
                    std::cin >> reps;
                    assert(reps >= 0);
                }

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

int main()
{
    const std::time_t now = std::time(nullptr);
    const std::tm calendar_time = *std::localtime( std::addressof(now) );
    MOTDProvider * provider; ///base class pointer
    if(calendar_time.tm_hour <= 12){
        ///morning MOTD
        provider = new MorningMOTD();
    } ///upcasting #1
    else {
        ///evening MOTD
        provider = new EveningMOTD();
    } ///upcasting #2
    std::cout << "Message of the day:" << "\n";
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
    if(exerciseMap.empty()){
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

        if( !(userCurrentChoice == 1 || userCurrentChoice == 2 || userCurrentChoice == 3) ){
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
                if(exerciseMap.size() == 0){
                    std::cout << "No exercises to delete!" << "\n";

                    continue;
                }
                ///delete an exercise altogether
                printAllExercises(std::cout, exerciseMap[ clients[clientID].getName() ]);
                std::cout << "Which exercise do you want to delete? (" << 1 << " to " << exerciseMap.size() << ")" << "\n";

                int indexToDelete;
                std::cout << "Number = ";
                std::cin >> indexToDelete;
                if( ! (1 <= indexToDelete && indexToDelete <= exerciseMap.size()) ){
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
                if(exerciseMap.size() == 0){
                    std::cout << "No exercises to modify!" << "\n";

                    continue;
                }

                printAllExercises(std::cout, exerciseMap[ clients[clientID].getName()]);
                std::cout << "Which exercise do you want to update? (" << 1 << " to " << exerciseMap.size() << ")" << "\n";

                int indexToUpdate;
                std::cout << "Number = ";
                std::cin >> indexToUpdate;
                if( ! (1 <= indexToUpdate && indexToUpdate <= exerciseMap.size()) ){
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
            if(exerciseMap.empty()){
                std::cout << "No logs detected yet!" << "\n";
            }
            else for (auto it : exerciseMap[ clients[clientID].getName() ]){
                ///std::cout << it.first << "!";
                it.second -> printRecords(std::cout, true);
                ///std::cout << "\n";
            }
        }
    }while(userCurrentChoice == 1 || userCurrentChoice == 2 || userCurrentChoice == 3);

    finExercises.close();

    std::ofstream foutExercises ("exercises.txt");
    outputExerciseMap(foutExercises, exerciseMap);
    foutExercises.close();


    std::ofstream foutClients("clients.txt");
    outputClientsLogs(foutClients, clients);
    foutClients.close();
    return 0;
}
