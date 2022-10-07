#include <iostream>
#include <string>
#include <conio.h>
#include <locale.h>
#include <time.h>
#include <windows.h>

using namespace std;

enum Sides
{
  UP,
  DOWN,
  RIGHT,
  LEFT,
  INVALID
};

struct Weapon
{
  int minimumDamage;
  int maximumDamage;
};

struct Player
{
  int level;
  int life;
  Weapon weapon;
  int coordinates[1][1];
};

struct Enemy
{
  string name;
  int life;
  Weapon weapon;
};

enum BlockType
{
  ROCK,
  PATH,
  ENEMY,
  PLAYER
};

struct Block
{
  BlockType blockType;
  Enemy *enemy = nullptr;
};

struct Map
{
  int height;
  int width;
  Block **blocks;
};

struct Phase
{
  string name;
  Map map;
  int amountOfEnemies;
  Enemy *enemies;
};

template <typename T>
bool isDead(T personage);
template <typename T, typename R>
R attack(T attacker, R defender);
Map createMap(int width, int height);
void playThePhase(Player player, Phase phase);
void movePlayer(Player player, Phase phase);
void renderMap(BlockType blockType);
int generateRandomNumber(int number);
Sides findPlayerSide(char key);
Phase createPhase(int amountOfEnemies, Enemy enemies[], int width, int height);

template <typename T>
bool isDead(T personage)
{
  if (personage.life < 0)
    return true;
  else
    return false;
}

template <typename T, typename R>
R attack(T attacker, R defender)
{
  int rangeDamage = attacker.weapon.maximumDamage - attacker.weapon.minimumDamage + 1;
  int damage = attacker.weapon.minimumDamage + rand() % rangeDamage;

  defender.life = defender.life - damage;

  return defender;
}

void playThePhase(Player player, Phase phase)
{
  cout << "Começou " << phase.name << endl
       << endl;

  for (int current = 0; current < 5; current++)
  {
    while (!isDead(phase.enemies[current]))
    {
      player = attack(phase.enemies[current], player);
      phase.enemies[current] = attack(player, phase.enemies[current]);

      cout << "O jogador atacou " << phase.enemies[current].name << " e ele ficou com " << phase.enemies[current].life << " de vida" << endl;
      cout << "O " << phase.enemies[current].name << " atacou o jogador ao mesmo tempo e o deixou com " << player.life << " de vida" << endl;

      if (isDead(player))
      {
        cout << "O jogador morreu, o jogo acabou";
        return;
      }
    }

    cout << phase.enemies[current].name << " foi morto" << endl
         << endl;
  }

  cout << "O jogador passou a fase";
}

int generateRandomNumber(int number)
{
  return rand() % number;
}

Map createMap(int width, int height)
{
  Block **blocks = new Block *[width];

  for (int i = 0; i < width; i++)
  {
    blocks[i] = new Block[height];
    for (int j = 0; j < height; j++)
    {
      Block block;
      int pickBlock = generateRandomNumber(100) + 1;

      block = {pickBlock >= 80 ? BlockType::ROCK : BlockType::PATH};

      blocks[i][j] = block;
    }
  }

  return {height, width, blocks};
}

Phase createPhase(int amountOfEnemies, Enemy *enemies, int width, int height)
{
  Map map = createMap(width, height);

  for (int i = 0; i < amountOfEnemies; i++)
  {
    int randomWidth, randomHeight;

    bool noEnemy = true;
    do
    {
      noEnemy = true;
      randomWidth = generateRandomNumber(width);
      randomHeight = generateRandomNumber(height);

      if (map.blocks[randomWidth][randomHeight].blockType == BlockType::PATH)
      {
        map.blocks[randomWidth][randomHeight] = {BlockType::ENEMY, (enemies + i)};
        noEnemy = false;
      }
    } while (noEnemy);
  }

  return {"Fase do Campo", map, amountOfEnemies, enemies};
}

void renderMap(BlockType blockType)
{
  switch (blockType)
  {
  case BlockType::ROCK:
  {
    cout << "O";
    // cout << "\U0001FAA8";
    break;
  }
  case BlockType::ENEMY:
  {
    cout << "E";
    // cout << "\U0001F479";
    break;
  }
  case BlockType::PATH:
  {
    cout << "=";
    // cout << "\U0001F43E";
    break;
  }
  default:
  {
    cout << "H";
    // cout << "\U0001F9B8";
    break;
  }
  }
}

Sides findPlayerSide(char key)
{
  if (kbhit())
    key = getch();

  if (key == 'w' || key == 'W')
    return Sides::UP;
  else if (key == 'a' || key == 'A')
    return Sides::LEFT;
  else if (key == 's' || key == 'S')
    return Sides::DOWN;
  else if (key == 'd' || key == 'D')
    return Sides::RIGHT;
  else
    return Sides::INVALID;
}

void movePlayer(Player player, Phase phase)
{
  bool alreadyPlayerOnMap = false;
  while (player.life > 0)
  {
    system("cls");
    char keyMovement = 0;

    for (int i = 0; i < phase.map.width; i++)
    {
      for (int j = 0; j < phase.map.height; j++)
      {
        if (!alreadyPlayerOnMap && phase.map.blocks[i][j].blockType == BlockType::PATH)
        {
          phase.map.blocks[i][j] = {BlockType::PLAYER, nullptr};
          alreadyPlayerOnMap = true;
        }

        renderMap(phase.map.blocks[i][j].blockType);
      }
      cout << endl;
    }

    Sides side = findPlayerSide(keyMovement);
  }
}

int main()
{
  srand(time(NULL));
  setlocale(LC_ALL, "Portuguese");

  Weapon enemyWeapon = {1, 5}, enemyPlayer = {4, 10};

  Enemy goblin = {"Goblin", 20, enemyWeapon},
        goblerto = {"Goblerto", 30, enemyWeapon},
        gobo = {"Gobo", 40, enemyWeapon},
        goblinio = {"Goblinio", 50, enemyWeapon},
        boss = {"Juca", 95, enemyWeapon};

  Player player = {1, 100, enemyPlayer, {0}};

  Enemy *enemies = new Enemy[5];

  for (int i = 0; i < 5; i++)
    enemies[i] = goblerto;

  Phase phase = createPhase(5, enemies, 6, 10);
  movePlayer(player, phase);
  // playThePhase(player, phase);
}