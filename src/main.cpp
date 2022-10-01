#include <iostream>
#include <string>
#include <time.h>

using namespace std;

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
  ENEMY
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
  Block blocks[6][10];
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
int generateRandomNumber(int number);
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
      int pickBlock = generateRandomNumber(5);

      if (!pickBlock)
        block = {BlockType::ROCK, nullptr};
      else
        block = {BlockType::PATH, nullptr};

      blocks[i][j] = block;
    }
  }

  return {height, width, **blocks};
}

Phase createPhase(int amountOfEnemies, Enemy *enemies, int width, int height)
{
  Map map = createMap(width, height);

  for (int i = 0; i < amountOfEnemies; i++)
  {
    int randomWidth = generateRandomNumber(width);
    int randomHeight = generateRandomNumber(height);

    do
    {
      randomWidth = generateRandomNumber(width);
      randomHeight = generateRandomNumber(height);

      if (map.blocks[randomWidth][randomHeight].blockType == BlockType::PATH)
      {
        map.blocks[randomWidth][randomHeight] = {BlockType::ENEMY, (enemies + i)};
        cout << (enemies + i)->name << endl;
      }
    } while (map.blocks[randomWidth][randomHeight].blockType != BlockType::PATH);
  }

  return {"Fase do Campo", map, amountOfEnemies, enemies};
}

int main()
{
  srand(time(NULL));

  Weapon enemyWeapon = {1, 5}, enemyPlayer = {4, 10};

  Enemy goblin = {"Goblin", 20, enemyWeapon},
        goblerto = {"Goblerto", 30, enemyWeapon},
        gobo = {"Gobo", 40, enemyWeapon},
        goblinio = {"Goblinio", 50, enemyWeapon},
        boss = {"Juca", 95, enemyWeapon};

  Player player = {1, 100, enemyPlayer};

  Phase phase;
  for (int i = 0; i < 5; i++)
    phase.enemies[i] = goblerto;

  // createPhase(5, phase.enemies, 6, 10);
  // playThePhase(player, phase);
}