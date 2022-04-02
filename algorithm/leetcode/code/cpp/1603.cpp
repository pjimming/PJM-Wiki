class ParkingSystem {
public:
    vector<int> tot;

    ParkingSystem(int big, int medium, int small) {
        tot = {big, medium, small};
    }
    
    bool addCar(int carType) {
        int t = carType - 1;
        if (tot[t] >= 1) {
            tot[t]--;
            return true;
        }
        return false;
    }
};

/**
 * Your ParkingSystem object will be instantiated and called as such:
 * ParkingSystem* obj = new ParkingSystem(big, medium, small);
 * bool param_1 = obj->addCar(carType);
 */