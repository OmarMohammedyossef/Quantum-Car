#include <iostream>
#include <memory>
#include <string>
using namespace std;

//================ Engine Interface =================
// Abstract base class (interface) for all engine types
// Defines common operations for any engine
class Engine {
public:
    virtual void increase() = 0;   // increase engine speed by 1 km/h
    virtual void decrease() = 0;   // decrease engine speed by 1 km/h
    virtual int getSpeed() const = 0; // return current engine speed
    virtual string getName() const = 0; // return engine type/name
    virtual ~Engine() {}
};

//================ Gasoline Engine =================
// Concrete implementation of Engine (Gas engine)
class GasolineEngine : public Engine {
    int speed = 0; // internal engine speed
public:
    void increase() override { speed++; } // increase speed by 1
    void decrease() override { if (speed > 0) speed--; } // prevent negative speed
    int getSpeed() const override { return speed; }
    string getName() const override { return "GasolineEngine"; }
};

//================ Electric Engine =================
// Concrete implementation of Engine (Electric engine)
class ElectronicEngine : public Engine {
    int speed = 0; // internal engine speed
public:
    void increase() override { speed++; }
    void decrease() override { if (speed > 0) speed--; }
    int getSpeed() const override { return speed; }
    string getName() const override { return "ElectronicEngine"; }
};

//================ Hybrid Engine =================
// Hybrid engine combines Gas + Electric engines
// Works with Electric below 50 km/h, Gas above 50 km/h
// Only one engine runs at a time (cost optimization)
class MixedHybridEngine : public Engine {
    GasolineEngine gas;       // internal gas engine
    ElectronicEngine electric; // internal electric engine
    int speed = 0; // overall hybrid speed

public:
    void increase() override {
        // Use electric engine at low speeds
        if (speed < 50) {
            electric.increase();
            gas.decrease(); // ensure gas engine is off
        } 
        // Switch to gas engine at higher speeds
        else {
            gas.increase();
            electric.decrease(); // ensure electric engine is off
        }
        speed++; // update hybrid speed
    }

    void decrease() override {
        // Use electric when speed is <= 50
        if (speed <= 50) {
            electric.decrease();
        } 
        // Use gas when speed > 50
        else {
            gas.decrease();
        }
        if (speed > 0) speed--; // prevent negative speed
    }

    int getSpeed() const override { return speed; }

    // Return current mode (Electric or Gas)
    string getName() const override {
        if (speed < 50) return "MixedHybridEngine (Electric Mode)";
        else return "MixedHybridEngine (Gas Mode)";
    }
};

//================ Car Class =================
// Represents a car that uses an Engine (composition)
// Car controls speed in steps of 20 km/h
class Car {
    unique_ptr<Engine> engine; // polymorphic engine
    int speed = 0; // car speed

public:
    // Constructor takes ownership of engine
    Car(unique_ptr<Engine> eng) : engine(move(eng)) {}

    // Replace current engine with a new one
    void replaceEngine(unique_ptr<Engine> newEngine) {
        engine = move(newEngine);
        cout << "Engine replaced successfully.\n";
    }

    // Start car at speed 0
    void start() {
        speed = 0;
        cout << "Car started with " << engine->getName() << endl;
    }

    // Stop car only if both car and engine speeds are 0
    void stop() {
        if (speed == 0 && engine->getSpeed() == 0) {
            cout << "Car stopped.\n";
        } else {
            cout << "Car must reach 0 speed before stopping!\n";
        }
    }

    // Increase speed by 20 km/h (using engine increase 20 times)
    void accelerate() {
        if (speed >= 200) { // max speed limit
            cout << "Max speed reached!\n";
            return;
        }

        // Increase engine speed step-by-step
        for (int i = 0; i < 20; i++) {
            engine->increase();
        }

        speed += 20;

        cout << "Accelerated to " << speed
             << " km/h using " << engine->getName() << endl;
    }

    // Decrease speed by 20 km/h
    void brake() {
        if (speed == 0) {
            cout << "Car already stopped.\n";
            return;
        }

        // Decrease engine speed step-by-step
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
// Factory Pattern to create cars with different engine types
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
// Demonstrates all car types and operations
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
    car3.accelerate(); // electric mode
    car3.accelerate(); // still electric
    car3.accelerate(); // switch to gas mode
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