#include <iostream>
#include <fstream>
#include <assert.h>

#include "fitness.h"

using namespace workoutDiary;

inline void printMainScreenOptions(std::ostream & out){
    out << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << "\n";
    out << "~ 1 = start a new workout           ~" << "\n";
    out << "~ 2 = modify records                ~" << "\n";
    out << "~ 3 = print current global PRs      ~" << "\n";
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

                    std::cout << "New exercise detected!" << "\n";
                    std::cout << "Would you like to add a description? (y/n)" << "\n";

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
                    assert(reps > 0);
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

void getExerciseMap(std::istream &in, std::map<std::string, exerciseType*> &exerciseMap){
    std::string currentExerciseName;
    while(in >> currentExerciseName){
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
        exerciseMap[currentExerciseName] = newEntry;
    }
}

void outputExerciseMap(std::ostream& out, std::map<std::string, exerciseType*> const & exerciseMap){
    for (auto it : exerciseMap){
        out << (it.second) -> getName() << "\n";
        (it.second) -> printRecords(out, false);
    }
}

int main()
{
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << "\n";
    std::cout << "!!Disclaimer: please use the singular form for each exercise         !!" << "\n";
    std::cout << "!!For example: \"dip\" and \"dips\" are registered as different exercises!!" << "\n";
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << "\n";
    std::cout << "\n";

    int userCurrentChoice = 0;
    ///cute little menu implementation
    std::ifstream fin ("test.txt");
    ///updateInput(fin);

    std::map<std::string, exerciseType*> exerciseMap;
    getExerciseMap(fin, exerciseMap);

    ///std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << "\n";
    std::cout << "Here are your current lifetime records:" << "\n";
    for (auto it : exerciseMap){
        ///std::cout << it.first << "!";
        it.second -> printRecords(std::cout, true);
        ///std::cout << "\n";
    }
    ///std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << "\n";
    std::cout << "\n";

    printMainScreenOptions(std::cout);
    std::cin >> userCurrentChoice;

    while(userCurrentChoice == 1 || userCurrentChoice == 2 || userCurrentChoice == 3){
        if(userCurrentChoice == 1){
            ///start a new workout
            menuNewWorkout(std::cin, exerciseMap);
        }
        else if(userCurrentChoice == 2){
            ///modify records
            printModifyScreenOptions(std::cout);
            std::cin >> userCurrentChoice;

            if(userCurrentChoice == 1){
                ///delete an exercise altogether
                printAllExercises(std::cout, exerciseMap);
                std::cout << "Which exercise do you want to delete? (" << 1 << " to " << exerciseMap.size() << ")" << "\n";

                int indexToDelete;
                std::cout << "Number = ";
                std::cin >> indexToDelete;
                if( ! (1 <= indexToDelete && indexToDelete <= exerciseMap.size()) ){
                    throw(std::runtime_error("Invalid number!"));
                }


                auto iteratorToDelete = exerciseMap.begin();
                for(int times = 1; times <= indexToDelete - 1; times++){
                    iteratorToDelete++;
                }
                std::cout << "***" << iteratorToDelete -> first << " deleted succesfully!***" << "\n" << "\n";
                delete iteratorToDelete -> second; ///free up memory
                exerciseMap.erase(iteratorToDelete);


            }
            else if(userCurrentChoice == 2){
                ///modify a certain PR manually
                printAllExercises(std::cout, exerciseMap);
                std::cout << "Which exercise do you want to update? (" << 1 << " to " << exerciseMap.size() << ")" << "\n";

                int indexToUpdate;
                std::cout << "Number = ";
                std::cin >> indexToUpdate;
                if( ! (1 <= indexToUpdate && indexToUpdate <= exerciseMap.size()) ){
                    throw(std::runtime_error("Invalid number!"));
                }


                auto iteratorToUpdate = exerciseMap.begin();
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
            for (auto it : exerciseMap){
                ///std::cout << it.first << "!";
                it.second -> printRecords(std::cout, true);
                ///std::cout << "\n";
            }
        }

        printMainScreenOptions(std::cout);
        std::cin >> userCurrentChoice;
    }

    fin.close();

    std::ofstream fout ("test.txt");
    outputExerciseMap(fout, exerciseMap);

    return 0;
}
