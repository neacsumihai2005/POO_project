#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <exception>

namespace workoutDiary{
    ///exceptions
    class exceptionDate : public std::exception{
        private:
            char * message = "Invalid Date! Try again.";
        public:
            char * what(){
                return message;
            }
    };

    ///the rest
    class Person{
        private:
            static int numberOfPersons;
        protected:
            std::string name;
        public:
            Person() = default;
            virtual ~Person() {} ///For the polymorphic deletion

            Person(std::string newName){
                name = newName;
            }

            std::string getName(){
                return name;
            }

            static void setNumberOfPersons(int x){
                numberOfPersons = x;
            }
            static void incNumberOfPersons(){
                numberOfPersons++;
            }
            static void defNumberOfPersons(){
                numberOfPersons--;
            }
            static int getNumberOfPersons(){
                return numberOfPersons;
            }

            virtual void setPassword(const std::string &newPassword) = 0;
            virtual std::string getPassword() const = 0;

            virtual void display(std::ostream & out) const{
                out << "Persoana " << name;
            }

            friend std::ostream& operator << (std::ostream& out, const Person& X){
                out << X.name;
                return out;
            }
            friend std::istream& operator >> (std::istream& in, Person& X){
                in >> X.name;
                return in;
            }
        protected:
            void outputName(std::ostream& out) const{
                out << name;
            }
    };
    int Person::numberOfPersons = 0;

    class Client : public Person{
        private:
            std::string password;
        public:
            Client() = default;
            Client(std::string newName) : Person(newName) {}
            Client(std::string newName, std::string newPassword) : Person(newName), password(newPassword){}

            std::string getName(){
                return static_cast<Person&> (*this).getName();
            }
            virtual void setPassword(const std::string& newPassword) override{
                password = newPassword;
            }
            virtual std::string getPassword() const override {
                return password;
            }

            virtual void display(std::ostream& out) const override{
                ///out << "Client " << static_cast<Person&>(*this).getName();
            }

            friend std::ostream& operator << (std::ostream& out, const Client & X){
                out << static_cast<const Person&> (X);
                return out;
            }
            friend std::istream& operator >> (std::istream& in, Client & X){
                in >> static_cast<Person&>(X);
                in >> X.password;

                return in;
            }

            void outputCredentials(std::ostream& out) const{
                outputName(out);
                out << ' ' ;
                out << password;
            }

    };

    class Manager : public Person{
        private:
            std::string email;
        public:
            Manager() = default;
            Manager(std::string newName) : Person(newName) {}
            Manager(std::string newName, std::string newEmail) : Person(newName), email(newEmail) {}

            std::string getName(){
                return static_cast<Person&> (*this).getName();
            }
            void setEmail(const std::string& newEmail){
                email = newEmail;
            }
            std::string getEmail(){
                return email;
            }

            friend std::ostream& operator << (std::ostream& out, const Manager & X){
                out << static_cast<const Person&> (X);
                return out;
            }
            friend std::istream& operator >> (std::istream& in, Manager & X){
                in >> static_cast<Person&>(X);
                in >> X.email;

                return in;
            }


    };

    class exerciseType{
        private:
            static int numberOfExercises;
            std::string name;
            std::string description;
            std::map<int, int> recordReps;
            ///recordReps[weight] = PR of repetitions for the weight

        public:
            exerciseType(){}
            exerciseType(std::string newName){
                name = newName;
            }
            exerciseType(std::string newName, std::string newDescription){
                name = newName;
                description = newDescription;
            }
            ~exerciseType() = default;

            static void setNumberOfExercises(int newNumberOfExercises){
                numberOfExercises = newNumberOfExercises;
            }
            static void incrementNumberOfExercises(){
                numberOfExercises = numberOfExercises + 1;
            }
            static void decrementNumberOfExercises(){
                numberOfExercises = numberOfExercises - 1;
            }
            static int getNumberOfExercises(){
                return numberOfExercises;
            }

            std::string getName(){
                return name;
            }
            void setName(std::string newName){
                name = newName;
            }

            std::string getDescription(){
                return description;
            }
            void setDescription(std::string newDescription){
                description = newDescription;
            }


            forceUpdatePR(int weight, int reps){
                if(recordReps.find(weight) == recordReps.end()){
                    recordReps[weight] = reps;
                }
                else {
                    recordReps[weight] = reps;
                }
            }
            tryUpdatePR(int weight, int reps){
                if(recordReps.find(weight) == recordReps.end()){
                    recordReps[weight] = reps;
                }
                else {
                    recordReps[weight] = std::max(
                                             recordReps[weight],
                                             reps
                                             );
                }
            }
            getPR(int weight){
                if(recordReps.find(weight) == recordReps.end()){
                    return 0;
                }
                return recordReps[weight];
            }

            void printRecords(std::ostream & out, bool userFriendly){
                if(userFriendly){
                    out << "- Records for the [" << name << "] exercise:" << "\n";
                    for(auto it : recordReps){
                        ///out << it.first << "kilograms: " << it.second << "\n";
                        out << "** " << it.second << " reps with : " << it.first << " kilograms" << "\n";
                    }
                    out << "\n";
                }
                else { ///developer friendly
                    out << recordReps.size() << "\n";
                    for(auto it : recordReps){
                        out << it.first << ' ' << it.second << "\n";
                    }
                }
            }
            void operator = (exerciseType const &operand){
                name = operand.name;
                description = operand.description;
                recordReps = operand.recordReps;
            }

    };

    class exercise{
        private:
            exerciseType * currentExerciseType;
            int weight;
            std::vector<int> listReps;

        public:
            exercise(){}

            exercise(exerciseType * newCurrentExerciseType, int newWeight, std::vector<int> newListReps){
                currentExerciseType = newCurrentExerciseType;
                weight = newWeight;
                listReps = newListReps;
            }

            ~exercise() = default;


            exerciseType * getCurrentExerciseType(){
                return currentExerciseType;
            }
            void setCurrentExercise(exerciseType * newCurrentExerciseType){
                currentExerciseType = newCurrentExerciseType;
            }

            int getWeight(){
                return weight;
            }
            void setWeight(int newWeight){
                weight = newWeight;
            }

            std::vector<int> getListReps(){
                return listReps;
            }
            void setListReps(std::vector<int> newListReps){
                listReps = newListReps;
            }

            void printReps(std::ostream & out){
                for(int i = 0; i < listReps.size(); i++){
                    out << listReps[i] << ' ';
                }
            }

            void addSet(int reps){
                listReps.push_back(reps);
            }

            void operator = (exercise const &operand){
                currentExerciseType = operand.currentExerciseType;
                weight = operand.weight;


                listReps.clear();
                for(int i = 0; i < operand.listReps.size(); i++){
                    listReps.push_back(operand.listReps[i]);
                }
            }
    };

    class calendaristicDate{
        private:
            int day;
            int month;
            int year;

        public:
            calendaristicDate(){}
            calendaristicDate(int newDay, int newMonth, int newYear){
                day = newDay;
                month = newMonth;
                year = newYear;
            }
            ~calendaristicDate() = default;

            int getDay(){
                return day;
            }
            void setDay(int newDay){
                day = newDay;
            }

            int getMonth(){
                return month;
            }
            void getMonth(int newMonth){
                month = newMonth;
            }

            int getYear(){
                return year;
            }
            void setYear(int newYear){
                year = newYear;
            }

            operator = (calendaristicDate const &operand){
                day = operand.day;
                month = operand.month;
                year = operand.year;
            }
            friend std::ostream& operator << (std::ostream& out, calendaristicDate const& X){
                if(X.day <= 9){
                    out << "0";
                } out << X.day;
                out << "/";
                if(X.month <= 9){
                    out << "0";
                } out << X.month;
                out << "/";
                out << X.year;

                return out;
            }
            friend std::istream& operator >> (std::istream& in, calendaristicDate & X){
                std::cout << "Day = "; in >> X.day;
                std::cout << "Month = "; in >> X.month;
                std::cout << "Year = "; in >> X.year;

                return in;
            }
            bool leapYear(){
                if (year % 400 == 0) {
                    return true;
                }
                else if(year % 100 == 0){
                    return false;
                }
                else if(year % 4 == 0){
                    return true;
                }
                else {
                    return false;
                }
            }
            bool validDate(){
                if(day < 0 || day > 31){
                    return false;
                }
                if(month < 0 || month > 12){
                    return false;
                }
                switch (month){
                    case 1: ///ianuarie
                        return (1 <= day && day <= 31);
                        break;
                    case 2: ///februarie
                        if(leapYear()){
                            return (1 <= day && day <= 29);
                        }
                        else {
                            return (1 <= day && day <= 28);
                        }
                        break;
                    case 3: ///martie
                        return (1 <= day && day <= 31);
                        break;
                    case 4: ///aprilie
                        return (1 <= day && day <= 30);
                        break;
                    case 5: ///mai
                        return (1 <= day && day <= 31);
                        break;
                    case 6: ///iunie
                        return (1 <= day && day <= 30);
                        break;
                    case 7: ///iulie
                        return (1 <= day && day <= 31);
                        break;
                    case 8: ///august
                        return (1 <= day && day <= 31);
                        break;
                    case 9: ///septembrie
                        return (1 <= day && day <= 30);
                        break;
                    case 10: ///octombrie
                        return (1 <= day && day <= 31);
                        break;
                    case 11: ///noiembrie
                        return (1 <= day && day <= 30);
                        break;
                    case 12: ///decembrie
                        return (1 <= day && day <= 31);
                        break;
                }
}
    };

    class workout{
        private:
            calendaristicDate date;
            std::vector<exercise*> listExercises;

        public:
            workout(calendaristicDate newDate){
                date = newDate;
            }
            workout(calendaristicDate newDate, std::vector<exercise*> newListExercises){
                date = newDate;
                listExercises = newListExercises;
            }
            ~workout() = default;


            void insertExercise(exercise* newExercise){
                listExercises.push_back(newExercise);
            }

            operator = (workout const &operand){
                date = operand.date;

                listExercises.clear();
                for(int i = 0; i < operand.listExercises.size(); i++){
                    listExercises.push_back(operand.listExercises[i]);
                }
            }
    };
}
