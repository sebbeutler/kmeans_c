#include "pcg.cl"

#define MAX_POINTS 100
#define MAX_SPECIES 4
#define UI_WIDTH 900
#define UI_HEIGHT 500

typedef struct Point
{
    double x;
    double y;
    int specie;
} Point;

typedef struct Specie
{
	int id;
	int color[3];
    Point centroid;
	Point points[MAX_POINTS];
	int point_count;
} Specie;

global pcg32u_random_t rand_seed;

global Point *_points = NULL;
global Specie *_species = NULL;

void specie_add(Specie *specie, Point* point);
kernel void kmeans_init(global Point* inputPoints, global Specie* inputSpecies, uint initstate, uint initseq);
int speciate();
double distance_point( Point *p1, Point *p2 );

void specie_add(Specie *specie, Point* point)
{
    specie->points[specie->point_count] = *point;
    specie->point_count++;
}

kernel void kmeans_init(global Point* inputPoints, global Specie* inputSpecies, uint initstate, uint initseq)
{
	pcg32_srandom_r(&rand_seed, initstate, initseq);

    _points = inputPoints;
    _species = inputSpecies;

	for (int i=0; i < MAX_POINTS; i++)
    	_points[i] = (Point) { UI_WIDTH*pcg32_doublerand(&rand_seed), UI_HEIGHT*pcg32_doublerand(&rand_seed), 0 };
	
	Point* c;
	Specie* s;
    for ( int i = 0; i < MAX_SPECIES; i++ )
	{
		c = &_points[pcg32u_boundedrand_r(&rand_seed, MAX_POINTS)];
		_species[i].centroid = *c;
		_species[i].id = i;
		_species[i].color[0] = pcg32u_boundedrand_r(&rand_seed, 255);
		_species[i].color[1] = pcg32u_boundedrand_r(&rand_seed, 255); 
		_species[i].color[2] = 255;
		specie_add(&_species[i], c);
	}

	speciate();
}

global int _step = 0;
kernel void kmeans_step()
{
    if (_step % 2 == 0)
    {
        _step++;
        for (int i=0; i < MAX_SPECIES; i++)
		{
            _species[i].centroid.x = 0;
            _species[i].centroid.y = 0;

			for (int j = 0; j < _species[i].point_count; j++)
			{
                _species[i].centroid.x += _species[i].points[j].x;
                _species[i].centroid.y += _species[i].points[j].y;
			}

            _species[i].centroid.x /= _species[i].point_count;
            _species[i].centroid.y /= _species[i].point_count;
		}
    }
    else
    {
        _step++;
		for (int i=0; i< MAX_SPECIES; i++)
		{
			_species[i].point_count = 0;
		}
        printf("Changes: %d\n", speciate());
    }
}

double distance_point( Point *p1, Point *p2 )
{
	return  sqrt( ( p1->x - p2->x ) * ( p1->x - p2->x ) +  ( p1->y - p2->y ) * ( p1->y - p2->y ) );
}

int speciate()
{
	int changes = 0;
	Specie* nearest_specie;
	double nearest_specie_distance = 0;
	double target_specie_distance;
	for (int i = 0; i < MAX_POINTS; i++)
    {
		nearest_specie_distance = 0;
				
        for (int j = 0; j < MAX_SPECIES; j++)
        {
			target_specie_distance = distance_point( &_species[j].centroid, &_points[i] );
			if ( target_specie_distance < nearest_specie_distance || nearest_specie_distance == 0 )
			{
				nearest_specie = &_species[j];
				nearest_specie_distance = target_specie_distance;
			}			
        }

		if (_points[i].specie != nearest_specie->id)
		{
			changes++;
			_points[i].specie = nearest_specie->id;
		}

		specie_add(nearest_specie, &_points[i]);
    }

	return changes;
}