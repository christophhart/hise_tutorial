var Follow = pc.createScript('follow');

Follow.attributes.add('target', {
    type: 'entity',
    title: 'Target',
    description: 'The Entity to follow'
});

Follow.attributes.add('distance', {
    type: 'number',
    default: 4,
    title: 'Distance',
    description: 'How far from the Entity should the follower be'
});

// initialize code called once per entity
Follow.prototype.initialize = function() {
    this.vec = new pc.Vec3();
};

// update code called every frame
Follow.prototype.update = function(dt) {
    if (!this.target) return;

    // get the position of the target entity
    var pos = this.target.getPosition();

    // calculate the desired position for this entity
    pos.x += 0.75 * this.distance;
    pos.y += 1.0 * this.distance;
    pos.z += 0.75 * this.distance;

    // smoothly interpolate towards the target position
    this.vec.lerp(this.vec, pos, 0.1);

    // set the position for this entity
    this.entity.setPosition(this.vec); 
};
