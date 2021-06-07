#include "Renderer.hpp"
#include "Ball.hpp"
#include "Bat.hpp"
#include "Utils.hpp"
#include "Neural_Network.hpp"
#include <iostream>
#include <thread>


void initRendering();
void drawingLoop();

void initGame();
void playerGameLoop();
void basicGameLoop();

void AIgameLoop();
void initAI();
void trainAI(int ai);

int main() {
    initGame();
    initAI();
    initRendering();
	return 0;
}

void initRendering() {
    sf::RenderWindow* cWindow = new sf::RenderWindow(sf::VideoMode(2000, 2000), "Pong!");
    Renderer::init(cWindow);
    while (cWindow->isOpen()) {
        

        sf::Event event;
        while (cWindow->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                cWindow->close();
                std::exit(0);
            }
        }
        cWindow->clear();//clear with every iteration

        //LOOP ORDER-------------------------------------------------------
        Renderer::loopBeforeDrawing();
        //playerGameLoop();
        AIgameLoop();
        Renderer::loopAfterDrawing();
        //AIgameLoop();
        //\LOOP ORDER-------------------------------------------------------

        cWindow->display();//display things drawn since clear() was called
    }
}
Ball* ball;
Bat** bats;
void initGame() {
    ball = new Ball(0.0f, 0.0f);
    bats = new Bat*[2];
    bats[0] = new Bat(-0.95f, 0.0f, 0.05f, 0.2f, 1);
    bats[1] = new Bat(0.9f, 0.0f, 0.05f, 0.2f, 2);
}
int collision = 0;
void playerGameLoop() {
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        bats[0]->move(true);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        bats[0]->move(false);
    }
    basicGameLoop();
    drawingLoop();
}
NeuralNetwork* ai1;
NeuralNetwork* ai2;
float punishment;
int iterations;
float* input;
int inputSize;
bool* canTrain;
void initAI() {
    inputSize = 6;
    ai1 = new NeuralNetwork(inputSize, 20, 1);
    ai2 = new NeuralNetwork(inputSize, 20, 1);
    //useful declarations for later
    punishment = 1.0f;
    iterations = 1000;
    input = new float[inputSize];
    canTrain = new bool[2];
    canTrain[0] = true;
    canTrain[1] = true;
}

void trainAI(int ai) {
    NeuralNetwork* cAI;
    if (ai == 0) cAI = ai1;
    else {
        cAI = ai2;
    }
    if (canTrain[ai] == true) {
        bool train = false;
        //right of left bat has collision with ball => + width
        if(ai == 0 && ball->getX() < -1.0f + bats[0]->getW()) {
            train = true;
        }
        //left of right bat has collision with balls right => + 2 * radius
        if (ai == 1 && ball->getX() > 1.0f - ball->getR() * 2) {
            train = true;
        }
        if (train == true) {//punish if enemy scored
            float* targets = new float[1];
            if (bats[ai]->getY() > ball->getY()) {
                targets[0] = ((ball->getY() - bats[ai]->getH() / 2) + 1.0f) / 2;
            }
            else {
                targets[0] = ((ball->getY() - bats[ai]->getH() / 2) + 1.0f) / 2;
            }
            canTrain[ai] = false;

            float punMult = Utils::calcDist1D(ball->getY(), bats[ai]->getY());
            punMult *= punishment;
            cAI->train(input, inputSize, targets, 1, punMult, iterations);

            delete[] targets;
        }
    }
}


void AIgameLoop() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        Renderer::loopBeforeDrawing();
        drawingLoop();
        Renderer::loopAfterDrawing();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        Renderer::loopBeforeDrawing();
        drawingLoop();
        Renderer::loopAfterDrawing();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }


    input[0] = (ball->getX() + 1.0f) / 2;
    input[1] = (ball->getY() + 1.0f) / 2;
    input[2] = (bats[0]->getX() + 1.0f) / 2;
    input[3] = (bats[0]->getY() + 1.0f) / 2;
    input[4] = ball->getXmotion();
    input[5] = ball->getYmotion();

    Matrix* ret = ai1->feedForward(input, inputSize);
    float out = ret->data[0][0] * 2 - 1.0f;
    if (out < bats[0]->getY()) {//move down
        bats[0]->move(out);
    }
    else {//move up
        bats[0]->move(out);
    }

    input[2] = (bats[1]->getX() + 1.0f) / 2;
    input[3] = (bats[1]->getY() + 1.0f) / 2;

    ret = ai2->feedForward(input, inputSize);
    out = ret->data[0][0] * 2 - 1.0f;
    if (out < bats[1]->getY()) {//move down
        bats[1]->move(out);
    }
    else {//move up
        bats[1]->move(out);
    }
    trainAI(0);
    trainAI(1);

    basicGameLoop();
}

//CALL AFTER AIGAMELOOP because it has the x >< 1.0f/-1.0f as well
void basicGameLoop() {
    ball->move();
    if (ball->getX() > -0.8f) bats[0]->setCollisionPossible(true);
    if (ball->getX() < 0.8f - (ball->getR() * 2)) bats[1]->setCollisionPossible(true);
    ball->collision(bats[0]);
    ball->collision(bats[1]);

    if (canTrain[0] == false || canTrain[1] == false) {
        if (ball->getX() > 1.1f || ball->getX() < -1.1f + ball->getR() * 2) {
            ball->setX(0.0f);
            ball->setY(0.0f);
            ball->resetMotion();
            canTrain[0] = true;
            canTrain[1] = true;
        }
    }
}

void drawingLoop() {
    Renderer::drawCircle(ball->getX(), ball->getY(), ball->getR(), sf::Color(255, 255, 0, 255), true);
    Renderer::drawRect(bats[0]->getX(), bats[0]->getY(), bats[0]->getW(), bats[0]->getH(), sf::Color(255, 0, 0, 255), true);
    Renderer::drawRect(bats[1]->getX(), bats[1]->getY(), bats[1]->getW(), bats[1]->getH(), sf::Color(0, 0, 255, 255), true);

}