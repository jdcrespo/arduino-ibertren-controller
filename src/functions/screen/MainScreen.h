#include "Screen.h"

class MainScreen
{
private:
  bool selectedParam;
  int accParam;
  int decParam;
  Screen &screen;

public:
  MainScreen(Screen &screenObj);
  int getAccParam();
  int getDecParam();
  void Draw(int currentPower,
            int currentSpeed,
            boolean currentDirection);
};
MainScreen::MainScreen(Screen &screenObj) : screen(screenObj)
{
  accParam = 200;
  decParam = 100;
}

int MainScreen::getAccParam()
{
  return accParam;
}

int MainScreen::getDecParam()
{
  return decParam;
}

void MainScreen::Draw(int currentPower,
                      int currentSpeed,
                      boolean currentDirection)
{
  int cursorValue = screen.getCursorValue();
  Serial.println(cursorValue);

  if (cursorValue != 0)
  {
    if (selectedParam)
    {
      decParam += cursorValue;
    }
    else
    {
      accParam += cursorValue;
    }
  }

  if (screen.getShortPress())
  {
    selectedParam = !selectedParam;
  }

  screen.getLCD().setCursor(0, 1);
  screen.getLCD().print("Pwr:");
  screen.getLCD().setCursor(9, 1);
  screen.getLCD().print("Dir:");

  int numBoxesFull = map(currentSpeed, 0, 100, 0, DISPLAY_COLUMNS);

  for (int i = 0; i < DISPLAY_COLUMNS; i++)
  {

    screen.getLCD().setCursor(i, 0);
    if (i <= numBoxesFull && currentSpeed > 0)
    {
      screen.getLCD().write(PROGRESS_BAR_FULL_ID);
    }
    else
    {
      if (i == 0)
      {
        screen.getLCD().write(PROGRESS_BAR_LEFT_EMPTY_ID);
      }
      else if (i == DISPLAY_COLUMNS - 1)
      {
        screen.getLCD().write(PROGRESS_BAR_RIGHT_EMPTY_ID);
      }
      else
      {
        screen.getLCD().write(PROGRESS_BAR_MIDDLE_EMPTY_ID);
      }
    }
  }

  int spacesRequired = 3 - ((String)currentPower).length();
  screen.getLCD().setCursor(4, 1);
  screen.getLCD().print("   ");

  screen.getLCD().setCursor(4 + spacesRequired, 1);

  screen.getLCD().print(currentPower);
  screen.getLCD().setCursor(7, 1);
  screen.getLCD().print("%");
  screen.getLCD().setCursor(14, 1);
  if (currentDirection)
  {
    screen.getLCD().print("->");
  }
  else
  {
    screen.getLCD().print("<-");
  }

  screen.getLCD().setCursor(1, 2);
  screen.getLCD().print("Acc:");
  screen.getLCD().setCursor(8, 2);
  screen.getLCD().print("   ");
  screen.getLCD().setCursor(8, 2);
  screen.getLCD().print((int)accParam);
  screen.getLCD().setCursor(1, 3);
  screen.getLCD().print("Dec:");
  screen.getLCD().setCursor(8, 3);
  screen.getLCD().print("   ");
  screen.getLCD().setCursor(8, 3);
  screen.getLCD().print((int)decParam);
  screen.getLCD().setCursor(0, 2);
  screen.getLCD().print(selectedParam ? " " : ">");
  screen.getLCD().setCursor(0, 3);
  screen.getLCD().print(selectedParam ? ">" : " ");
}
