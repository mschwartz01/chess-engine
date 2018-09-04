# Chess Engine
This is a chess engine.  More specifically, from a given chess position, the engine will determine a move to make.  This is done using an alpha-beta search algorithm.  A position is also given a score, determined by the pieces on the board.  The locations of the pieces are represented using bitboards.

## Acknowledgements
- Bluefever Software has a great YouTube series, [*Chess Engine in C*](https://www.youtube.com/watch?v=bGAfaepBco4&list=PLZ1QII7yudbc-Ky058TEaOstZHVbT-2hg), which I found helpful in learning many of the basics of chess programming.
- Logic Crazy Chess also has a helpful YouTube series, [*Programming an Advanced Java Chess Engine*](https://www.youtube.com/watch?v=V_2-LOvr5E8&list=PLQV5mozTHmacMeRzJCW_8K3qw2miYqd0c).
- I found the description of alpha-beta search in *Artificial Intelligence A Modern Approach* by Stuart Russell and Peter Norvig to be helpful.

## Future goals
- Implement the UCI protocol to make it easier for humans and other engines to play against my engine.
- Improve the move ordering to allow the alpha-beta search to run faster.
- Improve the evalution function to take into account piece location, game state (middle, beginning, or end), pawn structure, etc.