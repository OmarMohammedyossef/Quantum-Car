#include <iostream>
#include <memory>
#include <string>
using namespace std;

//================ Engine Interface =================
class Engine {
public:
    virtual void increase() = 0;
    virtual void decrease() = 0;
    virtual int getSpeed() const = 0;
    virtual string getName() const = 0;
    virtual ~Engine() {}
};

//================ Gasoline Engine =================
class GasolineEngine : public Engine {
    int speed = 0;
public:
    void increase() override { speed++; }
    void decrease() override { if (speed > 0) speed--; }
    int getSpeed() const override { return speed; }
    string getName() const override { return "GasolineEngine"; }
};

//================ Electric Engine =================
class ElectronicEngine : public Engine {
    int speed = 0;
public:
    void increase() override { speed++; }
    void decrease() override { if (speed > 0) speed--; }
    int getSpeed() const override { return speed; }
    string getName() const override { return "ElectronicEngine"; }
};

//================ Hybrid Engine =================
class MixedHybridEngine : public Engine {
    GasolineEngine gas;
    ElectronicEngine electric;
    int speed = 0;

public:
    void increase() override {
        if (speed < 50) {
            electric.increase();
            gas.decrease(); // ensure gas is off
        } else {
            gas.increase();
            electric.decrease(); // ensure electric is off
        }
        speed++;
    }

    void decrease() override {
        if (speed <= 50) {
            electric.decrease();
        } else {
            gas.decrease();
        }
        if (speed > 0) speed--;
    }

    int getSpeed() const override { return speed; }

    string getName() const override {
        if (speed < 50) return "MixedHybridEngine (Electric Mode)";
        else return "MixedHybridEngine (Gas Mode)";
    }
};

//================ Car Class =================
class Car {
    unique_ptr<Engine> engine;
    int speed = 0;

public:
    Car(unique_ptr<Engine> eng) : engine(move(eng)) {}

    void replaceEngine(unique_ptr<Engine> newEngine) {
        engine = move(newEngine);
        cout << "Engine replaced successfully.\n";
    }

    void start() {
        speed = 0;
        cout << "Car started with " << engine->getName() << endl;
    }

    void stop() {
        if (speed == 0 && engine->getSpeed() == 0) {
            cout << "Car stopped.\n";
        } else {
            cout << "Car must reach 0 speed before stopping!\n";
        }
    }

    void accelerate() {
        if (speed >= 200) {
            cout << "Max speed reached!\n";
            return;
        }

        for (int i = 0; i < 20; i++) {
            engine->increase();
        }

        speed += 20;
        cout << "Accelerated to " << speed
             << " km/h using " << engine->getName() << endl;
    }

    void brake() {
        if (speed == 0) {
            cout << "Car already stopped.\n";
            return;
        }

        for (int i = 0; i < 20; i++) {
            engine->decrease();
        }

        speed -= 20;
        if (speed < 0) speed = 0;

        cout << "Braked to " << speed
             << " km/h using " << engine->getName() << endl;
    }
};

//================ Factory =================
class CarFactory {
public:
    static Car createCar(const string& type) {
        if (type == "GasolineEngine")
            return Car(make_unique<GasolineEngine>());
        else if (type == "ElectronicEngine")
            return Car(make_unique<ElectronicEngine>());
        else if (type == "MixedHybridEngine")
            return Car(make_unique<MixedHybridEngine>());
        else
            throw invalid_argument("Invalid engine type");
    }
};

//================ Main =================
int main() {

    cout << "=== Gasoline Car ===\n";
    auto car1 = CarFactory::createCar("GasolineEngine");
    car1.start();
    car1.accelerate();
    car1.brake();
    car1.stop();

    cout << "\n=== Electric Car ===\n";
    auto car2 = CarFactory::createCar("ElectronicEngine");
    car2.start();
    car2.accelerate();
    car2.accelerate();
    car2.brake();
    car2.brake(); // reach 0
    car2.stop();

    cout << "\n=== Hybrid Car ===\n";
    auto car3 = CarFactory::createCar("MixedHybridEngine");
    car3.start();
    car3.accelerate(); // electric
    car3.accelerate(); // electric
    car3.accelerate(); // switch to gas
    car3.brake();
    car3.brake();
    car3.brake();
    car3.stop();

    cout << "\n=== Replace Engine Demo ===\n";
    car3.replaceEngine(make_unique<GasolineEngine>());
    car3.start();
    car3.accelerate();

    return 0;
}