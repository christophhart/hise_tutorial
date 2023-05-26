import * as THREE from "/three.module.js"

const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera( 45, window.innerWidth / window.innerHeight, 0.1, 100 );

const renderer = new THREE.WebGLRenderer({ alpha: true });

renderer.shadowMapEnabled = true;

// to antialias the shadow
renderer.shadowMapType = THREE.PCFSoftShadowMap;


renderer.setSize( window.innerWidth, window.innerHeight);
document.body.appendChild( renderer.domElement );

const geometry = new THREE.BoxGeometry( 1, 1, 1 );
const material = new THREE.MeshLambertMaterial( { color: 0x0088FF} );
const cube = new THREE.Mesh( geometry, material );

const light = new THREE.DirectionalLight(0x00fffc, 0.4)
light.position.z = 4;
light.position.y = 2;

scene.add(light);



scene.add( cube );

camera.position.z = 5;




window.updateZ = function(value)
{
    console.log(value);
    camera.position.z = value;
}


function animate() {
    requestAnimationFrame( animate );

    getX(2.5).then ((result) => 
    { 
    	cube.rotation.x += result; 
    });
    
    cube.rotation.y += 0.05;

    renderer.render( scene, camera );
}

animate();