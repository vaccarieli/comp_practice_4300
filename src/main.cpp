#include <iostream>

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

const float OBJECT_SPEED = 10.0f;

void updatePosition(const sf::Event& event, float& circleSpeedX, float& circleSpeedY)
{
    switch (event.key.code) 
    {
        case sf::Keyboard::Up:
        {
            circleSpeedY = -OBJECT_SPEED;
            std::cout << "Moved Up!" << std::endl;
            break;
        } 
        case sf::Keyboard::Down:
        {
            circleSpeedY = OBJECT_SPEED;
            std::cout << "Moved Down!" << std::endl;
            break;
        } 
        case sf::Keyboard::Left:
        {
            circleSpeedX = -OBJECT_SPEED;
            std::cout << "Moved Left!" << std::endl;
            break;
        } 
        case sf::Keyboard::Right:
        {
            circleSpeedX = OBJECT_SPEED;
            std::cout << "Moved Up!" << std::endl;
            break;
        } 
        default:
            break;
    }
}

void resetPositionVars(float& circleSpeedX, float& circleSpeedY) 
{
    circleSpeedX = 0;
    circleSpeedY = 0;
}

int main(int argc, char* argv[])
{
    // create a new window of size w*h pixels
    // top-left of the window is (0,0) and bottom-right  is (w,h)
    // you will have to read these from the config file
    const int wWidth = 800;
    const int wHeight = 600;
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML works!");
    window.setFramerateLimit(60);

    // initialize IMGUI and create a clock used for its internal timing 
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    //scale the imgui ui by a given factor, does not affect text size
    ImGui::GetStyle().ScaleAllSizes(1.0f);

    // the imgui color {r,g,b} wheel requires floats from 0-1 instead of ints from 0-255
    float c[3] = { 0.0f, 1.0f, 1.0f};

    // let's make the shape that we will draw to the screen 
    float circleRadius = 20; // radius to draw the circle
    int circleSegments = 64; // number of segments to draw the circle with
    float circleSpeedX = 0.0f; // we will use this to move the circle later 
    float circleSpeedY = 0.0f; // you will read these values from the file 
    bool drawCircle = true; // wheter or not to draw the circle
    bool drawText = true; // whether or not to draw the text

    // creat sfml circle shape based on our parameters
    sf::CircleShape circle(circleRadius, circleSegments); // create a circle with shape with radius 50

    circle.setPosition(10.0f, 10.0f);

    // let's load a font so we can display some text
    sf::Font myFont;

    // attempt to load the font from a file
    if(!myFont.loadFromFile("bin/fonts/tech.ttf"))
    {
        // if we can't load the font, print an error to the error console and exit
        std::cerr << "Could not load font!" << std::endl;
        exit(-1);
    }

    // set up the text object that will be drawn to the screen
    sf::Text text("Sample text", myFont, 24);

    // position the top-left corner of the text so that the text aligns on the bottom 
    // text character size is in pixels, so move text up from the bottom by its height
    text.setPosition(0, wHeight - (float)text.getCharacterSize());

    // set up a character array to set the text 
    char displayString[255] = "Sample Text";

    // main loop - continues for each frame while window is open
    while (window.isOpen())
    {
        // event handling
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            // pass the event to imgui to be parsed
            ImGui::SFML::ProcessEvent(window, event);

            // this event triggers when the window is closed
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }

            // this event is triggered when a key is pressed
            if(event.type == sf::Event::KeyPressed)
            {
                //print the key that was pressed to the console
                std::cout << "Key pressed with code = " << event.key.code << std::endl;

                //update position x & y
                updatePosition(event, circleSpeedX, circleSpeedY);

                // example, what happens when x is pressed 
                if(event.key.code == sf::Keyboard::X)
                {
                    //reverse the x direction of the circle on the screen 
                    circleSpeedX *= -1.0f;
                }
            }

        }

        // update imgui for this frame with the time that the last frame had
        ImGui::SFML::Update(window, deltaClock.restart());

        // draw the UI
        ImGui::Begin("Window Title");
        ImGui::Text("Windows Text!");
        ImGui::Checkbox("Draw Circle", &drawCircle);
        ImGui::SameLine();
        ImGui::Checkbox("Draw Text", &drawText);
        ImGui::SliderFloat("Radius", &circleRadius, 0.0f, 300.0f);
        ImGui::SliderInt("Sides", &circleSegments, 3, 64);
        ImGui::ColorEdit3("Color Circle", c);
        ImGui::InputText("Text", displayString, 255);

        if(ImGui::Button("Set Text"))
        {
            text.setString(displayString);
        }
        ImGui::SameLine();

        if(ImGui::Button("Reset Circle"))
        {
            circle.setPosition(0,0);
        }
        ImGui::End();

        // set the circle properties, because they may have been updated with the ui
        circle.setFillColor(sf::Color(c[0]*255, c[1]*255, c[2]*255));
        circle.setPointCount(circleSegments);
        circle.setRadius(circleRadius);

        // basic animation - move the each frame if it's still in frame 
        circle.setPosition(circle.getPosition().x + circleSpeedX, circle.getPosition().y + circleSpeedY);
        resetPositionVars(circleSpeedX, circleSpeedY);

        // basic rendering function calls 
        window.clear(); // clear the window of any previous drawn
        if(drawCircle) // draw the circle if true
        {
            window.draw(circle);
        }
        if(drawText) // draw the text if true
        {
            window.draw(text);
        }
        ImGui::SFML::Render(window); // Draw the UI last so it's on top
        window.display();
    }

    return 0;
}
