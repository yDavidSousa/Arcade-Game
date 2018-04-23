#include <SDL.h>
#include <collider.h>

int collider_circle_collision(circle_collider_t circleA, circle_collider_t circleB){
    return circleA.radius + circleB.radius >= mathf_distance(circleA.center, circleB.center) ? 1 : 0;
}

int collider_box_collision(box_collider_t boxA, box_collider_t boxB){
    return mathf_range_intersect(boxA.min.x, boxA.max.x, boxB.min.x, boxB.max.x)
           && mathf_range_intersect(boxA.min.y, boxA.max.y, boxB.min.y, boxB.max.y)? 1 : 0;
}

int collider_circle_point_collision(circle_collider_t circle, vector2_t point){
    return mathf_distance(circle.center, point) < circle.radius ? 1 : 0;
}

int collider_box_point_collision(box_collider_t boxcollider, vector2_t point){
    return mathf_range(point.x, boxcollider.min.x, boxcollider.min.x + boxcollider.max.x) &&
                   mathf_range(point.y, boxcollider.min.y, boxcollider.min.y + boxcollider.max.y) ? 1 : 0;
}

box_collider_t collider_new_box(float x, float y, float w, float h){
    box_collider_t boxCollider = {};
    boxCollider.min.x = x;
    boxCollider.min.y = y;
    boxCollider.size.x = w;
    boxCollider.size.y = h;
    boxCollider.extents = vector2_divide_scalar(boxCollider.size, 2);
    boxCollider.max = vector2_sum(boxCollider.min, boxCollider.size);
    return boxCollider;
}

void collider_update_box(box_collider_t *boxcollider, float x, float y){
    boxcollider->min.x = x;
    boxcollider->min.y = y;
    boxcollider->max.x = x + boxcollider->size.x;
    boxcollider->max.y = y + boxcollider->size.y;
}

box_collider_t collider_minkowski_diff_box(box_collider_t boxA, box_collider_t boxB){
    vector2_t topLeft = vector2_subtract(boxA.min, boxB.max);
    vector2_t fullSize = vector2_sum(boxA.size, boxB.size);

    return collider_new_box(topLeft.x, topLeft.y, fullSize.x, fullSize.y);
}

vector2_t collider_penetration_vector_box(box_collider_t boxcollider, vector2_t point){
    float minDist = mathf_abs(point.x - boxcollider.min.x);
    vector2_t boundsPoint = {boxcollider.min.x, point.y};

    //Finish checking x axis
    if(mathf_abs(boxcollider.max.x - point.x) < minDist){
        minDist = mathf_abs(boxcollider.max.x - point.x);
        boundsPoint.x = boxcollider.max.x;
        boundsPoint.y = point.y;
    }

    //Move to y axis
    if(mathf_abs(boxcollider.max.y - point.y) < minDist){
        minDist = mathf_abs(boxcollider.max.y - point.y);
        boundsPoint.x = point.x;
        boundsPoint.y = boxcollider.max.y;
    }

    if(mathf_abs(boxcollider.min.y - point.y) < minDist){
        minDist = mathf_abs(boxcollider.min.y - point.y);
        boundsPoint.x = point.x;
        boundsPoint.y = boxcollider.min.y;
    }

    return boundsPoint;
}

void collider_render_box(box_collider_t boxcollider, SDL_Renderer *renderer, color_t color){
    SDL_Rect boxToDraw = {};
    boxToDraw.x = (int)boxcollider.min.x;
    boxToDraw.y = (int)boxcollider.min.y;
    boxToDraw.w = (int)boxcollider.size.x;
    boxToDraw.h = (int)boxcollider.size.y;

    SDL_SetRenderDrawColor( renderer, (uint8_t) color.r, (uint8_t) color.g, (uint8_t) color.b, (uint8_t) color.a );
    SDL_RenderDrawRect( renderer, &boxToDraw );
}

void collider_render_fill_box(box_collider_t boxcollider, SDL_Renderer *renderer, color_t color){
    SDL_Rect boxToDraw = {};
    boxToDraw.x = (int)boxcollider.min.x;
    boxToDraw.y = (int)boxcollider.min.y;
    boxToDraw.w = (int)boxcollider.size.x;
    boxToDraw.h = (int)boxcollider.size.y;

    SDL_SetRenderDrawColor( renderer, (uint8_t) color.r, (uint8_t) color.g, (uint8_t) color.b, (uint8_t) color.a);
    SDL_RenderFillRect(renderer, &boxToDraw);
}