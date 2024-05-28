class MOTDProvider{
    public:
        std::string getIntro(){
            return "Message of the day";
        }
        virtual std::string getMOTD() const = 0;
        virtual ~MOTDProvider() {} //Virtual destructor for polymorphic behavior
};

class MorningMOTD : public MOTDProvider{
    public:
        std::string getMOTD() const override {
            return "Good morning! Have a wonderful day ahead!";
        }
};

class EveningMOTD : public MOTDProvider {
    public:
        std::string getMOTD() const override{
            return "Good evening~~ Relax and enjoy your evening!";
        }
};
