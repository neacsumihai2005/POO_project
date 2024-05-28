#include <windows.h> //WinApi header

class ConsoleColorManager{
    private:
        HANDLE hConsole;
        static ConsoleColorManager * instance;

        ConsoleColorManager(){ //Private constructor to prevent
            //instantiation from outside
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        }
    public:
        //Static method to access the single instance
        static ConsoleColorManager* getInstance(){
            if(instance == nullptr){
                instance = new ConsoleColorManager();
            }
            return instance;
        }

        void setConsoleColor(int colorKey){
            SetConsoleTextAttribute(hConsole, colorKey);
        }
};
ConsoleColorManager* ConsoleColorManager::instance = nullptr;


