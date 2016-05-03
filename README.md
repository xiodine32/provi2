Proiecte Vianiste
===

Now Linux compatible&trade;

## To build you will need
  * SDL (install _sdl_ 1.2 or 1.3 - not 2)
  * GL
  * GLU
  * pthreads


INPUT COMMANDS
---

    help                      - prints out stuff
    START REC                 - start receiving connections
    STOP REC                  - stop  receiving connections
    start                     - start contest
    timeleft                  - print time left
    setp<TEAM><##>            - set special problem for team
    add_<TEAM>|<POINTS>       - add points to team
    a<TEAM><PROBLEM><ANSWER>  - add problem to team

BACKUP FORMAT
---

Every tag is separated by a *|*

  1. time left in seconds
  2. number of teams
  3. number of problems
  4. for **each team**:
    * name
    * points
    * number of correct problems
    * bonus problem
    * number of answers for **each problem**
  5. for **each problem**:
    * number of points
    * number on answers
    * bonus points
    * for **each team**: if it's answered
