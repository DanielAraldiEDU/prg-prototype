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
  COORD coordinates;
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
  Map map;
  int amountOfEnemies;
  Enemy *enemies;
};

template <typename T>
bool isDead(T personage);
bool playThePhase(Player player, Enemy enemy);
int generateRandomNumber(int number);
Map createMap(int width, int height);
Phase createPhase(int amountOfEnemies, Enemy enemies[], int width, int height);
template <typename T, typename R>
R attack(T attacker, R defender);
Sides findPlayerSide(char key);
void movePlayer(Player player, Phase phase);
void renderMap(BlockType blockType);

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

bool playThePhase(Player player, Enemy enemy)
{
  cout << "Começou a Batalha!!" << endl
       << endl;

  while (!isDead<Enemy>(enemy))
  {
    player = attack<Enemy, Player>(enemy, player);
    enemy = attack<Player, Enemy>(player, enemy);

    cout << "O jogador atacou " << enemy.name << " e ele ficou com " << enemy.life << " de vida!" << endl;
    cout << "O " << enemy.name << " atacou o jogador ao mesmo tempo e o deixou com " << player.life << " de vida!" << endl;

    if (isDead<Player>(player))
    {
      cout << "O jogador morreu, o jogo acabou!";

      return false;
    }
  }

  cout << enemy.name << " foi morto!" << endl;
  cout << "O jogador ganhou a batalha!!!" << endl
       << endl;

  return true;
}

int generateRandomNumber(int number)
{
  return rand() % number;
}

Map createMap(int width, int height)
{
  Block **blocks = new Block *[height];

  for (int i = 0; i < height; i++)
  {
    blocks[i] = new Block[width];
    for (int j = 0; j < width; j++)
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

      if (map.blocks[randomHeight][randomWidth].blockType == BlockType::PATH)
      {
        map.blocks[randomHeight][randomWidth] = {BlockType::ENEMY, (enemies + i)};
        noEnemy = false;
      }
    } while (noEnemy);
  }

  return {map, amountOfEnemies, enemies};
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
  system("cls");

  bool alreadyPlayerOnMap = false;

  for (int i = 0; i < phase.map.height; i++)
  {
    for (int j = 0; j < phase.map.width; j++)
    {
      if (!alreadyPlayerOnMap)
      {
        int randomWidth = generateRandomNumber(phase.map.width);
        int randomHeight = generateRandomNumber(phase.map.height);
        if (phase.map.blocks[randomHeight][randomWidth].blockType == BlockType::PATH)
        {
          phase.map.blocks[randomHeight][randomWidth] = {BlockType::PLAYER, nullptr};
          player.coordinates.X = randomHeight;
          player.coordinates.Y = randomWidth;
          alreadyPlayerOnMap = true;
        }
      }

      renderMap(phase.map.blocks[i][j].blockType);
    }
    cout << endl;
  }

  while (player.life > 0)
  {
    char keyMovement = 0;
    int enemiesOnMap = 0;
    bool playerWon;

    Sides side = findPlayerSide(keyMovement);

    if (side == Sides::RIGHT && (phase.map.blocks[player.coordinates.X][player.coordinates.Y + 1].blockType == BlockType::ENEMY || phase.map.blocks[player.coordinates.X][player.coordinates.Y + 1].blockType == BlockType::PATH) && (player.coordinates.Y + 1) < phase.map.width)
    {
      system("cls");

      if (phase.map.blocks[player.coordinates.X][player.coordinates.Y + 1].blockType == BlockType::PATH)
      {
        player.coordinates.Y = player.coordinates.Y + 1;
        phase.map.blocks[player.coordinates.X][player.coordinates.Y - 1] = {BlockType::PATH, nullptr};
        phase.map.blocks[player.coordinates.X][player.coordinates.Y] = {BlockType::PLAYER, nullptr};
      }
      else
      {
        playerWon = playThePhase(player, *(phase.map.blocks[player.coordinates.X][player.coordinates.Y + 1].enemy));
        if (playerWon)
        {
          player.coordinates.Y = player.coordinates.Y + 1;
          phase.map.blocks[player.coordinates.X][player.coordinates.Y - 1] = {BlockType::PATH, nullptr};
          phase.map.blocks[player.coordinates.X][player.coordinates.Y] = {BlockType::PLAYER, nullptr};
        }
        else
        {
          return;
        }
      }

      for (int i = 0; i < phase.map.height; i++)
      {
        for (int j = 0; j < phase.map.width; j++)
        {
          renderMap(phase.map.blocks[i][j].blockType);
        }
        cout << endl;
      }
    }
    else if (side == Sides::LEFT && (phase.map.blocks[player.coordinates.X][player.coordinates.Y - 1].blockType == BlockType::ENEMY || phase.map.blocks[player.coordinates.X][player.coordinates.Y - 1].blockType == BlockType::PATH) && (player.coordinates.Y - 1) >= 0)
    {
      system("cls");

      if (phase.map.blocks[player.coordinates.X][player.coordinates.Y - 1].blockType == BlockType::PATH)
      {
        player.coordinates.Y = player.coordinates.Y - 1;
        phase.map.blocks[player.coordinates.X][player.coordinates.Y + 1] = {BlockType::PATH, nullptr};
        phase.map.blocks[player.coordinates.X][player.coordinates.Y] = {BlockType::PLAYER, nullptr};
      }
      else
      {
        playerWon = playThePhase(player, *(phase.map.blocks[player.coordinates.X][player.coordinates.Y - 1].enemy));
        if (playerWon)
        {
          player.coordinates.Y = player.coordinates.Y - 1;
          phase.map.blocks[player.coordinates.X][player.coordinates.Y + 1] = {BlockType::PATH, nullptr};
          phase.map.blocks[player.coordinates.X][player.coordinates.Y] = {BlockType::PLAYER, nullptr};
        }
        else
        {
          return;
        }
      }

      for (int i = 0; i < phase.map.height; i++)
      {
        for (int j = 0; j < phase.map.width; j++)
        {
          renderMap(phase.map.blocks[i][j].blockType);
        }
        cout << endl;
      }
    }
    else if (side == Sides::UP && (phase.map.blocks[player.coordinates.X - 1][player.coordinates.Y].blockType == BlockType::ENEMY || phase.map.blocks[player.coordinates.X - 1][player.coordinates.Y].blockType == BlockType::PATH) && (player.coordinates.X - 1) >= 0)
    {
      system("cls");

      if (phase.map.blocks[player.coordinates.X - 1][player.coordinates.Y].blockType == BlockType::PATH)
      {
        player.coordinates.X = player.coordinates.X - 1;
        phase.map.blocks[player.coordinates.X + 1][player.coordinates.Y] = {BlockType::PATH, nullptr};
        phase.map.blocks[player.coordinates.X][player.coordinates.Y] = {BlockType::PLAYER, nullptr};
      }
      else
      {
        playerWon = playThePhase(player, *(phase.map.blocks[player.coordinates.X - 1][player.coordinates.Y].enemy));
        if (playerWon)
        {
          player.coordinates.X = player.coordinates.X - 1;
          phase.map.blocks[player.coordinates.X + 1][player.coordinates.Y] = {BlockType::PATH, nullptr};
          phase.map.blocks[player.coordinates.X][player.coordinates.Y] = {BlockType::PLAYER, nullptr};
        }
        else
        {
          return;
        }
      }

      for (int i = 0; i < phase.map.height; i++)
      {
        for (int j = 0; j < phase.map.width; j++)
        {
          renderMap(phase.map.blocks[i][j].blockType);
        }
        cout << endl;
      }
    }
    else if (side == Sides::DOWN && (phase.map.blocks[player.coordinates.X + 1][player.coordinates.Y].blockType == BlockType::ENEMY || phase.map.blocks[player.coordinates.X + 1][player.coordinates.Y].blockType == BlockType::PATH) && (player.coordinates.X + 1) < phase.map.width)
    {
      system("cls");

      if (phase.map.blocks[player.coordinates.X + 1][player.coordinates.Y].blockType == BlockType::PATH)
      {
        player.coordinates.X = player.coordinates.X + 1;
        phase.map.blocks[player.coordinates.X - 1][player.coordinates.Y] = {BlockType::PATH, nullptr};
        phase.map.blocks[player.coordinates.X][player.coordinates.Y] = {BlockType::PLAYER, nullptr};
      }
      else
      {
        playerWon = playThePhase(player, *(phase.map.blocks[player.coordinates.X + 1][player.coordinates.Y].enemy));
        if (playerWon)
        {
          player.coordinates.X = player.coordinates.X + 1;
          phase.map.blocks[player.coordinates.X - 1][player.coordinates.Y] = {BlockType::PATH, nullptr};
          phase.map.blocks[player.coordinates.X][player.coordinates.Y] = {BlockType::PLAYER, nullptr};
        }
        else
        {
          return;
        }
      }

      for (int i = 0; i < phase.map.height; i++)
      {
        for (int j = 0; j < phase.map.width; j++)
        {
          renderMap(phase.map.blocks[i][j].blockType);
        }
        cout << endl;
      }
    }

    for (int i = 0; i < phase.map.height; i++)
    {
      for (int j = 0; j < phase.map.width; j++)
      {
        if (phase.map.blocks[i][j].blockType == BlockType::ENEMY)
          enemiesOnMap++;
      }
    }

    if (!enemiesOnMap)
    {
      cout << endl;
      system("pause");
      system("cls");
      cout << "Parabêns, você venceu a fase!!" << endl;
      return;
    }
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

  Player player = {1, 100, enemyPlayer, {0, 0}};

  Enemy *enemies = new Enemy[5];

  for (int i = 0; i < 5; i++)
    enemies[i] = goblerto;

  movePlayer(player, createPhase(5, enemies, 10, 6));
}