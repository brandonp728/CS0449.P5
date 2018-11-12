#include <stdio.h>
#include <string.h>

char* fix_string(char* string_to_fix);

int main()
{
  int point = 0;
  int pointBool = 0;
  printf("Welcome to the Casino! Hope you don't lose all your money, that'd suck\n");
  printf("What is your name: ");
  char name[100];
  fgets(name, sizeof(name), stdin);
  strcpy(name, fix_string(name));
  printf("%s, would you like to play or quit? ", name);
  char play_or_quit[100];
  fgets(play_or_quit, sizeof(play_or_quit), stdin);
  FILE* dev_dice = fopen("/dev/dice", "r");
  if(dev_dice == NULL)
  {
    printf("That file doesn't exist!");
    return 1;
  }
  strcpy(play_or_quit, fix_string(play_or_quit));
  while(strcmp(play_or_quit, "play")==0)
  {
    unsigned char rollOne, rollTwo;
    fread(&rollOne, sizeof(rollOne), 1, dev_dice);
    fread(&rollTwo, sizeof(rollTwo), 1, dev_dice);
    int correctOne = (int)rollOne+1;
    int correctTwo = (int)rollTwo+1;
    int total = correctOne + correctTwo;
    printf("You rolled %d + %d = %d\n", correctOne, correctTwo, total);
    if(total == 7 || total == 11)
    {
      if(pointBool && total == 7)
      {
        printf("You lose!\n");
        pointBool = 0;
      }
      else
      {
        printf("You win!\n");
      }
    }
    else if((total == 2 || total == 3 || total == 12) && pointBool == 0)
    {
      printf("You lose!\n");
    }
    else if(total == point && pointBool)
    {
      printf("You win!\n");
      pointBool = 0;
    }
    else
    {
      printf("You rolled a point! Keep going until you win or lose\n");
      point = total;
      pointBool = 1;
      strcpy(play_or_quit, "play");
    }
    if(pointBool == 0)
    {
      printf("Keeping playing or quit? (Play or Quit)\n");
      fgets(play_or_quit, sizeof(play_or_quit), stdin);
      strcpy(play_or_quit, fix_string(play_or_quit));
    }
  }
  printf("Goodbye\n");
  return 0;
}

char* fix_string(char* string_to_fix)
{
  int length = strlen(string_to_fix);
  if(length > 0 && string_to_fix[length - 1] == '\n')
  {
      string_to_fix[length - 1] = '\0';
  }
  return string_to_fix;
}
