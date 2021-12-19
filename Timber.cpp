#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

// Left or Right
enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];


int main()
{
    VideoMode vm(1920, 1080);

    RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
   // Window::setMouseCursorVisible(false);
    window.setMouseCursorVisible(false);

    // Create a texture to hold a graphics on the GPU
    Texture textureBackground;

    // Load a graphics
    textureBackground.loadFromFile("graphics/background.png");

    // Create a sprite
    Sprite spriteBackground;

    // Attach the texture to the sprite
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 0);

    // Make a tree sprite
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    // Prepare the bee
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);

    bool beeActive{ false };
    float beeSpeed = 0.0f;

    // make 3 cloud sprites
    Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");

    // 3 New sprites with the same texture
    Sprite spriteCloud1(textureCloud);
    Sprite spriteCloud2(textureCloud);
    Sprite spriteCloud3(textureCloud);

    // Position the clouds on the left of the screen
    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(0, 250);
    spriteCloud3.setPosition(0, 500);

    // Are the cluds currently on screen?
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;

    // How fast is each clud?
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    // Variables to control time itself
    Clock clock;

    // pause
    bool paused = true;

    // Draw some text
    int score = 0;
    
    Text messageText;
    Text scoreText;

    // include font
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    // set font
    messageText.setFont(font);
    scoreText.setFont(font);

    // Assing message
    messageText.setString("Press 'Enter' to start!");
    scoreText.setString("Score = 0");

    // Make it big
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    // Choose a color
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);

    // Position the text
    FloatRect textRect = messageText.getLocalBounds();

    messageText.setOrigin(textRect.left +
        textRect.width / 2.0f,
        textRect.top +
        textRect.height / 2.0f);

    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);

    // Time bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color(255,255,200,250));
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    // Prepare 6 branches
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");

    // Set the texture 
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        // Set the sprite's origin to dead centre
        branches[i].setPosition(-2000, -2000);

        branches[i].setOrigin(220, 20);
    }

    // Prepare the player
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);
    side playerSide = side::LEFT;

    // Prepare the gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    // Prepare the axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe(textureAxe);
    spriteAxe.setPosition(700, 830);

    // Line the axe up width the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Prepare the flying log
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog(textureLog);
    spriteLog.setPosition(810, 720);

    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    // Controll the player input
    bool acceptInput = false;

    // Prepare the sounds
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/chop.wav");
    Sound chop(chopBuffer);

    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/death.wav");
    Sound death(deathBuffer);

    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("sound/out_of_time.wav");
    Sound outOfTime(ootBuffer);

    Text escText;
    escText.setFont(font);


    while (window.isOpen())
    {
        /*
        * Handle the players input
        */
        
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased && !paused)
            {
                acceptInput = true;
                // hide the axe
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            paused = false;

            // Reset the time and the score
            score = 0;
            timeRemaining = 6.0f;

            for (int i = 1; i < NUM_BRANCHES; ++i)
            {
                //updateBranches(i);
                branchPositions[i] = side::NONE;
            }

            // Make sure the gravestone is hidden
            spriteRIP.setPosition(675, 2000);
            //Move Player into positions
            spritePlayer.setPosition(580, 720);

            acceptInput = true;
        }

        // Wrap the player controls to
        if (acceptInput)
        {
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                // Make sure the player is on the right
                playerSide = side::RIGHT;

                score++;
                // Add to the amount of time remaining
                timeRemaining += (2 / score) + 0.15;

                spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(1200, 720);

                // Update the branches
                updateBranches(score);

                // Set the log flying to the left
                spriteLog.setPosition(810, 720);
                logSpeedX = -5000;
                logActive = true;

                acceptInput = false;

                // Play a chop sound
                chop.play();
            }

            // Handel the left cursor key
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                playerSide = side::LEFT;

                score++;

                timeRemaining += (2 / score) + 0.15;

                spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(580, 720);

                updateBranches(score);

                spriteLog.setPosition(810, 720);
                logSpeedX = 5000;
                logActive = true;

                acceptInput = false;

                // Play a chop sound
                chop.play();
             }
        }

        /*
        * Update the scene
        */

        if (!paused)
        {

            // Measure time
            Time dt = clock.restart();

            // Substract from the amount of time remaning
            timeRemaining -= dt.asSeconds();
            // Size up the bar
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining,
                timeBarHeight));

            if (timeRemaining <= 0.0f)
            {
                // Pause the game
                paused = true;

                // Change the message shoun to the player
                messageText.setString("Out of time!!");
                escText.setString("Press 'Esc' for exit");
                FloatRect escRect = escText.getLocalBounds();
                escText.setOrigin(escRect.left +
                    escRect.width / 2.0f,
                    escRect.top + escRect.height / 2.0f);
                escText.setPosition(1920 / 2.0, 1080 / 2.0f + 65);

                // Reposition the text
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left +
                    textRect.width / 2.0f,
                    textRect.top +
                    textRect.height / 2.0f);

                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                // Play the out of time sound
                outOfTime.play();
            }

            // Setup the bee
            if (!beeActive)
            {
                // How fast is the bee
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;

                // How high is the bee
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(1920, height);

                beeActive = true;
            }
            else
                // Move the bee
            {
                spriteBee.setPosition(
                    spriteBee.getPosition().x -
                    (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);

                if (spriteBee.getPosition().x < -100)
                {
                    beeActive = false;
                }
            }

            // Manage the clouds
            // Cloud 1
            if (!cloud1Active)
            {
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200);

                srand((int)time(0) * 10);
                float height = (rand() % 150);
                spriteCloud1.setPosition(-200, height);
                cloud1Active = true;
            }
            else
            {
                spriteCloud1.setPosition(
                    spriteCloud1.getPosition().x + 
                    (cloud1Speed * dt.asSeconds()),
                    spriteCloud1.getPosition().y);

                if (spriteCloud1.getPosition().x > 1920) {
                    // Set it up ready to be a whole new cloud next frame 
                    cloud1Active = false;
                }
            }

            // Cloud 2
            if (!cloud2Active)
            {
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200);

                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150;
                spriteCloud2.setPosition(-200, height);
                cloud2Active = true;
            }
            else
            {
                spriteCloud2.setPosition(
                    spriteCloud2.getPosition().x + 
                    (cloud2Speed * dt.asSeconds()),
                    spriteCloud2.getPosition().y);

                if (spriteCloud2.getPosition().x > 1920) {
                    // Set it up ready to be a whole new cloud next frame 
                    cloud2Active = false;
                }
            }

            // Cloud 3
            if (!cloud3Active)
            {
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200);

                srand((int)time(0) * 30);
                float height = (rand() % 450) - 150;
                spriteCloud3.setPosition(-200, height);
                cloud3Active = true;
            }
            else
            {
                spriteCloud3.setPosition(
                    spriteCloud3.getPosition().x +
                    (cloud3Speed * dt.asSeconds()),
                    spriteCloud3.getPosition().y);

                if (spriteCloud3.getPosition().x > 1920) {
                    // Set it up ready to be a whole new cloud next frame 
                    cloud3Active = false;
                }
            }

            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());
            
            for (int i = 0; i < NUM_BRANCHES; ++i)
            {
                float height = i * 150;

                if (branchPositions[i] == side::LEFT)
                {
                    branches[i].setPosition(610, height);
                    branches[i].setOrigin(220, 40);
                    branches[i].setRotation(180);
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    branches[i].setPosition(1330, height);
                    branches[i].setOrigin(220, 40);
                    branches[i].setRotation(0);
                }
                else if (branchPositions[i]== side::NONE)
                {
                    branches[i].setPosition(3000, height);
                }
            }

            // Handle a flying log
            if (logActive)
            {
                spriteLog.setPosition(
                    spriteLog.getPosition().x +
                    (logSpeedX * dt.asSeconds()),

                    spriteLog.getPosition().y +
                    (logSpeedY * dt.asSeconds())
                );

                // Has the log reaxhed the right hand edge?
                if (spriteLog.getPosition().x < -300 ||
                    spriteLog.getPosition().x > 2000)
                {
                    logActive = false;
                    //spriteLog.setPosition(810, 720);
                }
            }

            // has the player been squished by a branch?
           if (branchPositions[5] == playerSide)
            {
                // death
                paused = true;
                acceptInput = false;

                spriteRIP.setPosition(525, 760);

                spritePlayer.setPosition(2000, 660);

                messageText.setString("SQUISHED!!");

                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left +
                    textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                escText.setString("Press 'Esc' for exit");
                FloatRect escRect = escText.getLocalBounds();
                escText.setOrigin(escRect.left +
                    escRect.width / 2.0f,
                    escRect.top + escRect.height / 2.0f);
                escText.setPosition(1920 / 2.0, 1080 / 2.0f + 65);

                // Play the death sound
                death.play();
            }
        }



        /*
        * Draw the scene-
        */

        // Clear everything from the last frame
        window.clear();

        // Draw our game scene here
        window.draw(spriteBackground);
        
        // Draw the clouds
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);

        // Draw branches
        for (int i = 0; i < NUM_BRANCHES; ++i)
        {
            window.draw(branches[i]);
        }

        // Draw the tree
        window.draw(spriteTree);

        window.draw(spritePlayer);
        window.draw(spriteAxe);
        window.draw(spriteLog);
        window.draw(spriteRIP);

        // Draw the insect
        window.draw(spriteBee);

        // Draw the score
        window.draw(scoreText);

        // Draw the time bar
        window.draw(timeBar);

        if (paused)
        {
            // Draw message
            window.draw(messageText);
            window.draw(escText);
        }

        // Show evryfing we just drew
        window.display();

    }

    return 0;
}

void updateBranches(int seed)
{
    for (int j = NUM_BRANCHES - 1; j > 0; j--)
    {
        branchPositions[j] = branchPositions[j - 1];
    }

    // Spawn a new branch
    srand((int)time(0) + seed);
    int r = (rand() % 5);

    switch (r)
    {
    case 0:
        branchPositions[0] = side::LEFT;
        break;
    case 1:
        branchPositions[0] = side::RIGHT;
        break;
    default:
        branchPositions[0] = side::NONE;
        break;
    }
}