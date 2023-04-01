#ifndef SCOREBOARD_H
#define SCOREBOARD_H

typedef struct Scoreboard Scoreboard;
typedef struct ScoreboardEntry ScoreboardEntry;

Scoreboard* scoreboard_create(int entries_number);
void scoreboard_destroy(Scoreboard* scoreboard);
void scoreboard_draw(Scoreboard* scoreboard);
ScoreboardEntry* scoreboard_get_entry(Scoreboard* scoreboard);

#endif /* SCOREBOARD_H */
