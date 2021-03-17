#include "common.h"

Bodies::Bodies(
    int gnumBodies,
    double maxSpeed,
    double maxMass,
    double spread,
    bool godPoint, 
    bool draw, 
    int frameDelay, 
    int drawSize, 
    int windowHeight, 
    int windowWidth){

    this->gnumBodies = gnumBodies; 
    this->maxSpeed = maxSpeed;
    this->maxMass = maxMass;
    this->spread = spread;
    this->godPoint = godPoint;
    this->draw = draw;
    this->frameDelay = frameDelay;
    this->drawSize = drawSize;
    this->windowHeight = windowHeight;
    this->windowWidth = windowWidth;
    this->running = true;

    p = (point_t *)malloc(sizeof(point_t)*gnumBodies);
    v = (point_t *)malloc(sizeof(point_t)*gnumBodies);
    m = (double *)malloc(sizeof(double)*gnumBodies);
    initialize_bodies();

    if(draw){
        if(SDL_Init(SDL_INIT_VIDEO) < 0){
            std::cout << SDL_GetError() << "\n";
            running = false;
        }
        else{
            window = SDL_CreateWindow("N-body simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
            if(window ==  NULL){
                std::cout << SDL_GetError() << "\n";
                running = false;
            }
            else{
                renderer = SDL_CreateRenderer(window, -1, 0);
                if(renderer == NULL){
                    std::cout << SDL_GetError() << "\n";
                    running = false;
                }
            }

        }
    }
}

Bodies::~Bodies(){
    free(p);
    free(v);
    free(m);
    if(draw)
        SDL_Quit();
}

void Bodies::initialize_bodies(){
    std::default_random_engine re;
    std::uniform_real_distribution<double> distX(windowWidth/2 - windowWidth/2 * spread/100, windowWidth/2 + windowWidth/2 * spread/100);
    std::uniform_real_distribution<double> distY(windowHeight/2 - windowHeight/2 * spread/100, windowHeight/2 + windowHeight/2 * spread/100);
    std::uniform_real_distribution<double> distV(-1.0 * maxSpeed, maxSpeed);
    std::uniform_real_distribution<double> distM(1.0, maxMass);

    for(int i = 0; i < gnumBodies; i++){
        p[i].x = distX(re);
        p[i].y = distY(re);
        v[i].x = distV(re);
        v[i].y = distV(re);
        m[i] = distM(re);
    }
    if(godPoint){
        m[0] = 100000000.0;
        p[0].x = windowWidth/2;
        p[0].y = windowHeight/2;
        v[0].x = 0.0;
        v[0].y = 0.0;
    }
}

void Bodies::draw_bodies(){
    while(SDL_PollEvent(&event) != 0){
        if(event.type == SDL_QUIT)
            running = false;
    }
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);
    
    if(godPoint){
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x30, 0x30, 0xFF);
        SDL_Rect rect = {(int)p[0].x, (int)p[0].y, DRAW_SIZE*2, DRAW_SIZE*2};
        SDL_RenderFillRect(renderer, &rect);
    }
    
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for(int i = 0 + godPoint; i < gnumBodies; i++){
        SDL_Rect rect = {(int)p[i].x, (int)p[i].y, DRAW_SIZE, DRAW_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(frameDelay);
}

double point_distance(point_t a, point_t b){
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double calc_magnitude(double a, double b, double distance, double gravity, double softening){
    return (gravity*a*b) / pow(distance + softening, 2);
}

point_t point_direction(point_t a, point_t b){
    point_t direction;
    direction.x = b.x - a.x;
    direction.y = b.y - a.y;
    return direction;
}

point_t point_deltav(point_t f, double m, double deltaTime){
    point_t deltav;
    deltav.x = f.x/m * deltaTime;
    deltav.y = f.y/m * deltaTime;
    return deltav;
}

point_t point_deltap(point_t v, point_t deltav, double deltaTime){
    point_t deltap;
    deltap.x = (v.x + deltav.x/2) * deltaTime;
    deltap.y = (v.y + deltav.y/2) * deltaTime;
    return deltap;
}

int read_int_argument(int argc, const char **argv, std::string identifier, int defaul_value){
    int arg = defaul_value;
    for(int i = 1; i < argc; i++)
        if(identifier == argv[i])
            arg = atoi(argv[i+1]);
    return arg;
}

bool read_bool_argument(int argc, const char **argv, std::string identifier, bool default_value){
    bool arg = default_value;
    for(int i = 1; i < argc; i++)
        if(identifier == argv[i])
            arg = true;
    return arg;
}

std::string read_string_argument(int argc, const char **argv, std::string identifier, std::string default_value){
    std::string arg = default_value;
    for(int i = 1; i < argc; i++)
        if(identifier == argv[i])
            arg = argv[i+1];
    return arg;
}

std::string help_message(){
    std::string message = "";
    message.append("N-bodies simulation\n\n");
    message.append("-h        : displays this helper text\n");
    message.append("-b <x>    : set number of bodies to x. default = " + DEFAULT_NUM_BODIES + '\n');
    message.append("-n <x>    : set number of steps to x. default = " + DEFAULT_NUM_STEPS + '\n');
    message.append("-d        : draw the point in each iteration. default = false\n");
    message.append("-g        : make point[0] a god point with 100000000x more mass than the others. default = false\n");
    message.append("-w <x>    : set number of workers (threads) to simultainiusly run the simulation. default = 4\n");
    message.append("\n");
    return message;
}
