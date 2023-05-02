#include <vector>
#include <bitset>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>
#include <map>


namespace Lab
{
    // data
    struct Position2D
    {
        long long int xPosition;
        long long int yPosition;
    };

    inline std::ostream& operator<<(std::ostream& os, const Position2D& position) {
        
        os << "xPos:" << position.xPosition << " yPos:" << position.yPosition << " ";
        return os; }

    inline bool operator<(const Position2D &firstPos, const Position2D &secondPos){ // TODO could be implemented as > for different order
        // first sort by y Position then by x Position...
        if (firstPos.yPosition < secondPos.yPosition){
            return true;
        }
        else if (firstPos.yPosition == secondPos.yPosition){
            return firstPos.xPosition < secondPos.xPosition;
        }
        return false;
    }

    inline bool operator==(const Position2D &firstPos, const Position2D &secondPos)
    {
        return firstPos.xPosition == secondPos.xPosition && firstPos.yPosition == secondPos.yPosition;
    }

    struct TakenPaths{
        bool topPath;
        bool rightPath;
        bool leftPath;
        bool bottomPath;
    };

    inline std::ostream& operator<<(std::ostream& os, const TakenPaths& path) {
        os << "top " << path.topPath << " right " << path.rightPath << " bottom " << path.bottomPath << " left " 
        << path.leftPath;
        return os;
    }


    inline bool operator==(const TakenPaths& firstPath, const TakenPaths& secondPath){
        return  firstPath.topPath == secondPath.topPath &&
                firstPath.rightPath == secondPath.rightPath &&
                firstPath.leftPath == secondPath.leftPath &&
                firstPath.bottomPath == secondPath.bottomPath;
    }

    inline TakenPaths takePath(const Position2D fromPos, const Position2D toPos, TakenPaths takenPaths);

    inline TakenPaths combinePaths(const TakenPaths path1, const TakenPaths path2);
    
    inline bool pathAlreadyTaken(const Position2D fromPos, const Position2D toPos,const TakenPaths takenPaths);
    inline bool differentPathAlreadyTaken(const Position2D fromPos, const Position2D toPos,const TakenPaths takenPaths);


    inline TakenPaths mirrorPath(TakenPaths path){
        // switch top and bottom without local variables
        // path.topPath = path.bottomPath xor path.topPath;
        // path.bottomPath = path.topPath xor path.bottomPath;
        // path.topPath = path.bottomPath xor path.topPath;

        // path.rightPath = path.leftPath xor path.rightPath;
        // path.leftPath = path.rightPath xor path.leftPath;
        // path.rightPath = path.leftPath xor path.rightPath;
        path.topPath = path.bottomPath ^ path.topPath;
        path.bottomPath = path.topPath ^ path.bottomPath;
        path.topPath = path.bottomPath ^ path.topPath;

        path.rightPath = path.leftPath ^ path.rightPath;
        path.leftPath = path.rightPath ^ path.leftPath;
        path.rightPath = path.leftPath ^ path.rightPath;
        return path;
    }

    // connected Positions
    struct Edge{
        Position2D firstPos;
        Position2D secondPos;
    };

    // sorted like a 2D field
    inline bool operator<(const Edge &firstEdge, const Edge &secondEdge){
        return firstEdge.firstPos < secondEdge.firstPos || firstEdge.secondPos < secondEdge.secondPos;
    }


    inline bool operator==(const Edge &firstEdge, const Edge &secondEdge)
    {
        return (firstEdge.firstPos == secondEdge.firstPos && firstEdge.secondPos == secondEdge.secondPos ) ||
                (firstEdge.firstPos == secondEdge.secondPos && firstEdge.secondPos == secondEdge.firstPos);
    }


    using BlockField2D = std::vector<std::vector<bool>>; // first x coordiante then y coordinate
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
        nothing,
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
        static int adjacentPoint(const Position2D point1, const Position2D point2);
        static std::vector<Position2D> enclosedPoints(const bool xPos, const Position2D firstPos, const Position2D secondPos);
        static Position2D nearestPoint(const Position2D point, const Way severalPoints);
        static bool adjacentToWay(const Position2D point, const Way way);
        static int timesAdjacentToWay(const Position2D point, const Way severalPoints);
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


    static Way combineWays(const Way firstWay, const Way secondWay){
        Way resultWay = firstWay;
        resultWay.insert(resultWay.begin(),secondWay.begin(),secondWay.end());
        return resultWay;
    }


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
        static Labyrinth2D connectSomeShapes2(const Labyrinth2D lab2D);
        static Labyrinth2D connectAllShapes(const Labyrinth2D lab2D);
        static Labyrinth2D connectAllShapes2(const Labyrinth2D lab2D);
        

        static Labyrinth2D depthSearchLabyrinth(const Labyrinth2D lab2d);
    };

    class LabyrinthSolver
    {
        private:
            static Way findConnectedTiles(const Labyrinth2D lb2D);
            static Way findConnectedTiles(const BlockField2D blockField, Position2D startPosition);

        public:
            static Way findWay(const Labyrinth2D lb2D);
            static Way findConnectedTilesSet(const Labyrinth2D lb2D);
            static Way findConnectedTilesSet(const BlockField2D blockField,Position2D startPosition);
            static WaysVector findAllWays(const Labyrinth2D lb2D);
            static WaysVector findAllWays(const BlockField2D blockfield);
            static WaysVector findAllWays2(const BlockField2D blockfield);
            
            static bool hasLoops(const Way way); 
            //static Way findLoopPositions(const Way way); 
            static Way freeNeighbors(const BlockField2D field, const Position2D position,const int extendBorder);
            static Way freeNeighborsDifferentIsland(const BlockField2D field, const Way island,const Position2D position,const int extendBorder);
            static Way newCombinedIsland(const BlockField2D field, const Way island,const Position2D position,const int extendBorder);
            static std::vector<std::pair<Position2D,int>> connectableIslandIndices(const BlockField2D& field
                     ,const std::map<Position2D,int>& posToSet,const std::map<int,int>& setIndexToIsland ,const Position2D& position,const int extendBorder);

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
        static void print2DLabyrinthToFile(const std::string fileName,const std::vector<std::vector<bool>> &inputVector, const std::vector<std::vector<bool>> &inputVector2,const std::string freeFieldEmojiUnicode,const std::string notFreeEmojiUnicode,const bool printIndices);

    };

}