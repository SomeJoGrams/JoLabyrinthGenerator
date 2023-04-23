#include <vector>
#include <bitset>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>


namespace Lab
{
    // data
    struct Position2D
    {
        long long int xPosition;
        long long int yPosition;
    };

    inline bool operator<(const Position2D &firstPos, const Position2D &secondPos){
        return firstPos.xPosition < secondPos.xPosition || firstPos.yPosition < secondPos.yPosition;
    }

    inline bool operator==(const Position2D &firstPos, const Position2D &secondPos)
    {
        return firstPos.xPosition == secondPos.xPosition && firstPos.yPosition == secondPos.yPosition;
    }

    using BlockField2D = std::vector<std::vector<bool>>;
    /* TODO is that bad abstraction? */
    using Way = std::vector<Position2D>;
    using WaysVector = std::vector<Way>;
    
    struct Labyrinth2D
    {
        Position2D startPosition;
        Position2D goalPosition;
        BlockField2D blockField;
    };

    enum class Pattern
    {
        random,
        insideOut,
        outsideIn,
        smart,
        empty
    };

    struct RandomSteps
    { // todo adapat for higher dimensions
        int posStepSize;
        int negStepSize;
        int startValue;
        int posValueAccept;
    };

    class PositionInterface
    {
    public:
        static int onSameLine(const Position2D firstPos, const Position2D secondPos);
        static bool betweenTwoPoints(const Position2D point, const Position2D firstPos, const Position2D secondPos);
        static long long int pointDistance(const Position2D point1, const Position2D point2);
        static std::vector<Position2D> enclosedPoints(const bool xPos, const Position2D firstPos, const Position2D secondPos);
        static Position2D nearestPoint(const Position2D point, const Way severalPoints);
    };

    // struct BlockField2D{
    //     std::vector<std::vector<bool>> blockField;
    // };
    //typedef std::vector<std::vector<bool>> BlockField2D; // using is better bc it allows easier templating + readability

    class RandomWalk
    {
    private:
        const RandomSteps randomStepObject;
        int currentWalkPosition;

    public:
        RandomWalk(RandomSteps randSteps);
        //~RandomWalk();

        int generateStep();
        int generateStepWithinRange(int startPosition, int startRange, int endRange);
        void clearCurrentWalk();
    };

    /*
*   should be able to fill Spaces based on a Pattern
*   
*
*/
    class Labyrinth2DGenerator
    {
    public:
        static Labyrinth2D generateLabyrinth(const Pattern generationPattern, const int xSize, const int ySize);
        static BlockField2D patternGeneration(BlockField2D blockField, const Pattern generationPattern);
        static BlockField2D fillLines(BlockField2D blockField, const Position2D pos1, const Position2D pos2, const bool filling);
        static BlockField2D fillPoints(const BlockField2D blockField,const Way pointsToFill, const bool filling);
        static Labyrinth2D mergeLabyrinths(const Labyrinth2D firstLabyrinth, const Labyrinth2D secondLabyrinth);
        static std::vector<std::vector<int>> randomWalk(const int xSize,const int ySize,const int randomWalkCount, const int stepsPerRandomWalk, RandomWalk &randWalker);
        static BlockField2D firstLabAlgorithm(std::vector<std::vector<int>> blockField);
        static BlockField2D generateEmptyBlockfield(const int xSize, const int ySize);
        static Labyrinth2D connectSomeShapes(const Labyrinth2D lab2D);
        static Labyrinth2D depthSearchLabyrinth(const Labyrinth2D lab2d);
    };

    class LabyrinthSolver
    {
        private:
            static Way findConnectedTiles(const Labyrinth2D lb2D);
        public:
            static Way findWay(const Labyrinth2D lb2D);
            static Way findConnectedTilesSet(const Labyrinth2D lb2D);
            static WaysVector findAllWays(const Labyrinth2D lb2D);

    };

    struct Labyrinth3D
    {
        Labyrinth2D startLabyrinth;
        Labyrinth2D endLabyrinth;

        std::vector<Labyrinth2D> labyrinths;
    };

    void printPosition();

    class PrintInterface
    {
    public:
        template <typename Val>
        static void print2DContainer(const std::vector<std::vector<Val>> &inputVector);
        static void print2DLabyrinthToFile(const std::string fileName, const std::vector<std::vector<bool>> &inputVector,const std::string freeFieldEmojiUnicode,const std::string notFreeEmojiUnicode);
        static void print2DLabyrinthToFile(const std::string fileName,const std::vector<std::vector<bool>> &inputVector, const std::vector<std::vector<bool>> &inputVector2,const std::string freeFieldEmojiUnicode,const std::string notFreeEmojiUnicode);
    };

}