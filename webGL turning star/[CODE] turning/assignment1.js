
var gl;

var theta = 0.0;
var thetaLoc;
var program;

window.onload = function init()
{
    var canvas = document.getElementById( "gl-canvas" );
    
    gl = WebGLUtils.setupWebGL( canvas );
    if ( !gl ) { alert( "WebGL isn't available" ); }

    //Configure WebGL

    gl.viewport( 0, 0, canvas.width, canvas.height);
	gl.clearColor(90/256, 135/256, 160/256, 1.0);    

    //Load shaders and initialize 
    program = initShaders( gl, "vertex-shader", "fragment-shader" );

	gl.useProgram( program );   

    //create a buffer on gpu and bind point    
    var bufferId = gl.createBuffer();
    gl.bindBuffer( gl.ARRAY_BUFFER, bufferId ); 

    //Associate out shader variables with our data buffer   	
	//attribute variable
    var vPosition = gl.getAttribLocation( program, "vPosition" );
    gl.vertexAttribPointer( vPosition, 2, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray( vPosition );

	//uniform variable
	var fColor = gl.getUniformLocation(program, "fColor");	

	//clear buffer bit
    gl.clear( gl.COLOR_BUFFER_BIT );
	
	render2(gl, fColor)
};

function landscape(gl, fColor){
	/*--------------------------------------------------------------------------------------------------- */
	/* Ground --------------------------------------------------------------------------------------------- */
	/*--------------------------------------------------------------------------------------------------- */

	//point
	var ground = new Float32Array([
		-1, -0.5,
		 1, -0.5,
		-1, -1,
		 1, -0.5,
		-1, -1,
		1, -1
	]);

	//buffer input
	gl.bufferData(gl.ARRAY_BUFFER, ground, gl.STATIC_DRAW );	
	
	// color
	gl.uniform4fv(fColor,[78/256,60/256,35/256,1]); // color (R,G,B,A)	
	var first = 0 // the starting index in the array of vector points.
	var count = 6 // the number of indices to be rendered.
	render(first, count) // render function

	tree_generator(gl, fColor, 0.9, -1.04 ,-0.05)

	/*--------------------------------------------------------------------------------------------------- */
	/* house --------------------------------------------------------------------------------------------- */
	/*--------------------------------------------------------------------------------------------------- */

	//point
	var house = new Float32Array([
		-0.85, 0.0,
		 0.0, 0.0,
		  -0.85, -0.5, // triangle
		0.0, 0.0,
		 -0.85, -0.5,
		  0.0, -0.5     // triangle
	]);

	//buffer input
	gl.bufferData(gl.ARRAY_BUFFER, house, gl.STATIC_DRAW );	
	
	// color
	gl.uniform4fv(fColor,[232/256,227/256,228/256,1]); // color (R,G,B,A)	
	var first = 0 // the starting index in the array of vector points.
	var count = 6 // the number of indices to be rendered.
	render(first, count) // render function

	/*--------------------------------------------------------------------------------------------------- */
	/* door --------------------------------------------------------------------------------------------- */
	/*--------------------------------------------------------------------------------------------------- */

	//point
	var door = new Float32Array([
		-0.3, -0.25,
		 -0.1, -0.25,
		 -0.3, -0.5, // triangle
		-0.1, -0.25,
		-0.3, -0.5,
		  -0.1, -0.5     // triangle
	]);

	//buffer input
	gl.bufferData(gl.ARRAY_BUFFER, door, gl.STATIC_DRAW );	
	
	// color
	gl.uniform4fv(fColor,[245/256,185/256,17/256,1]); // color (R,G,B,A)	
	var first = 0 // the starting index in the array of vector points.
	var count = 6 // the number of indices to be rendered.
	render(first, count) // render function

	/*--------------------------------------------------------------------------------------------------- */
	/* window --------------------------------------------------------------------------------------------- */
	/*--------------------------------------------------------------------------------------------------- */

	//point
	var window = new Float32Array([
		-0.4, -0.05,
		-0.1, -0.05,
		-0.4, -0.2, // triangle
		-0.1, -0.05,
		-0.4, -0.2,
		-0.1, -0.2,     // triangle

		-0.75, -0.05,
		-0.45, -0.05,
		-0.75, -0.2, // triangle
		-0.45, -0.05,
		-0.75, -0.2,
		-0.45, -0.2     // triangle
	]);

	//buffer input
	gl.bufferData(gl.ARRAY_BUFFER, window, gl.STATIC_DRAW);	
	
	// color
	gl.uniform4fv(fColor,[170/256, 210/256, 220/256, 1.0]); // color (R,G,B,A)	
	var first = 0 // the starting index in the array of vector points.
	var count = 12 // the number of indices to be rendered.
	render(first, count/2) // render function
	render(count/2, count) // render function

	/*--------------------------------------------------------------------------------------------------- */
	/* roof --------------------------------------------------------------------------------------------- */
	/*--------------------------------------------------------------------------------------------------- */

	//point
	var roof = new Float32Array([
		-0.85, 0.2,
		 0.0, 0.2,
		  -0.9, 0.0, // triangle
		0.0, 0.2,
		 -0.9, 0.0,
		  0.05, 0.0     // triangle
	]);

	//buffer input
	gl.bufferData(gl.ARRAY_BUFFER, roof, gl.STATIC_DRAW );	
	
	// color
	gl.uniform4fv(fColor,[175/256,50/256,50/256,1]); // color (R,G,B,A)	
	var first = 0 // the starting index in the array of vector points.
	var count = 6 // the number of indices to be rendered.
	render(first, count) // render function

	/*--------------------------------------------------------------------------------------------------- */
	/* pond --------------------------------------------------------------------------------------------- */
	/*--------------------------------------------------------------------------------------------------- */

	//point
	var pond = new Float32Array([
		0.6, -0.85,
		0.6, -0.6,
		0.8, -0.65,
		0.85, -0.75,
		0.8, -0.85,
		0.6, -0.9,
		0.4, -0.85,
		0.35, -0.75,
		0.4, -0.65,
		0.6, -0.6
		]);

	//input buffer
	gl.bufferData(gl.ARRAY_BUFFER, pond, gl.STATIC_DRAW );

	// color
	gl.uniform4f(fColor, 230/256, 250/256, 250/256, 1); // color (R,G,B,A)
	gl.drawArrays( gl.TRIANGLE_FAN, 0, 10 ); // render function
}
function star_generator(gl, fColor, size, x_offset, y_offset){
	/*--------------------------------------------------------------------------------------------------- */
	/* star --------------------------------------------------------------------------------------------- */
	/*--------------------------------------------------------------------------------------------------- */

	//point
	var star = new Float32Array([
		0.0, 0.0,
		0, 0.9,
		0.2, 0.3,
		0.9, 0.3,
		0.35, -0.1,
		0.6, -0.8,
		0, -0.4,
		-0.6, -0.8,
		-0.35, -0.1,
		-0.9, 0.3,
		-0.2, 0.3,
		0, 0.9
		]);
	
	//세타값을 조정해서 움직이는 것 처럼 보이게 하는 부분
	for(i=0;i<star.length;i+=2){
		var pre_x = star[i]
		var pre_y = star[i+1]
		
		tetha = 10
		star[i] = Math.cos(tetha)*pre_x - Math.sin(tetha)*pre_y
		star[i+1] = Math.sin(tetha)*pre_x + Math.cos(theta)*pre_y
	}

	//별의 사이즈와 위치 조정
	for(i=0; i<star.length; i++){
		star[i]*=size;
		if(i%2==0){ 
			star[i]+=x_offset;
		}
		else{ 
			star[i]+=y_offset;}
	}
	//input buffer
	gl.bufferData(gl.ARRAY_BUFFER, star, gl.STATIC_DRAW);	

	gl.uniform4fv(fColor,[245/256,185/256,17/256,1]); // color (R,G,B,A)	
	gl.drawArrays( gl.TRIANGLE_FAN, 0, 12);// render function
}


function tree_generator(gl, fColor, size, x_offset, y_offset){
	/*--------------------------------------------------------------------------------------------------- */
	/* leaf --------------------------------------------------------------------------------------------- */
	/*--------------------------------------------------------------------------------------------------- */

	//point
	var leaf = new Float32Array([
		0.2, 0.5, 
		-0.05, 0.25,
		 0.45, 0.25,   // triangle 
		0.2, 0.25,
		 -0.05, 0,
		  0.45, 0,     // triangle 
		0.2, 0,
		 -0.05, -0.25,
		  0.45, -0.25  // triangle 
		]);

	for(i=0; i<leaf.length; i++){
		leaf[i]*=size;
		if(i%2==0) leaf[i]+=x_offset;
		else leaf[i]+=y_offset;
	}

	//input buffer
	gl.bufferData(gl.ARRAY_BUFFER, leaf, gl.STATIC_DRAW);	
	
	// color
	gl.uniform4fv(fColor,[79/256,97/256,65/256,1]); // color (R,G,B,A)	
	gl.drawArrays( gl.TRIANGLES, 0, 9);// render function

	/*--------------------------------------------------------------------------------------------------- */
	/* body --------------------------------------------------------------------------------------------- */
	/*--------------------------------------------------------------------------------------------------- */

	//point
	var body = new Float32Array([
		0.125, -0.25,
		 0.275, -0.25,
		  0.125, -0.5, // triangle
		0.275, -0.25,
		 0.125, -0.5,
		  0.275, -0.5     // triangle
		]);

	for(i=0; i<body.length; i++){
		body[i]*=size;
		if(i%2==0) body[i]+=x_offset;
		else body[i]+=y_offset;
	}

	//input buffer
	gl.bufferData(gl.ARRAY_BUFFER, body, gl.STATIC_DRAW);

	// color
	gl.uniform4f(fColor, 209/256, 188/256, 131/256, 1); // color (R,G,B,A)
	gl.drawArrays( gl.TRIANGLES, 0, 6); // render function
}

function render(first, count) {
	gl.drawArrays( gl.TRIANGLES, first, count );
}

// render2 -> 0.1초마다 캔버스에 모든 오브젝트를 새로 그리는 랜더링 함수
function render2(gl, fColor){
	landscape(gl, fColor)
	tree_generator(gl, fColor, 0.8, 0.4 ,-0.1)
	tree_generator(gl, fColor, 0.7, 0.08 ,-0.2)
	tree_generator(gl, fColor, 0.6, -0.85 ,-0.4)
	tree_generator(gl, fColor, 0.55, -0.7 ,-0.65)
	tree_generator(gl, fColor, 0.55, 0.72 ,-0.3)
	star_generator(gl, fColor, 0.1, 0.5 ,0.7)
	star_generator(gl, fColor, 0.05, -0.9 ,0.9)
	star_generator(gl, fColor, 0.06, -0.7 ,0.83)
	star_generator(gl, fColor, 0.04, 0.7 ,0.9)
	star_generator(gl, fColor, 0.025, -0.35 ,0.65)
	star_generator(gl, fColor, 0.035, -0.45 ,0.6)
	
	setTimeout( function() {
		gl.clear( gl.COLOR_BUFFER_BIT );

		theta += 0.1;
		gl.uniform1f( thetaLoc, theta );

		landscape(gl, fColor)
		tree_generator(gl, fColor, 0.8, 0.4 ,-0.1)
		tree_generator(gl, fColor, 0.7, 0.08 ,-0.2)
		tree_generator(gl, fColor, 0.6, -0.85 ,-0.4)
		tree_generator(gl, fColor, 0.55, -0.7 ,-0.65)
		tree_generator(gl, fColor, 0.55, 0.72 ,-0.3)
		star_generator(gl, fColor, 0.1, 0.5 ,0.7)
		star_generator(gl, fColor, 0.05, -0.9 ,0.9)
		star_generator(gl, fColor, 0.06, -0.7 ,0.83)
		star_generator(gl, fColor, 0.04, 0.7 ,0.9)
		star_generator(gl, fColor, 0.025, -0.35 ,0.65)
		star_generator(gl, fColor, 0.035, -0.45 ,0.6)
		
		//setInterval(render,10);
		requestAnimFrame(render2(gl, fColor));
	}, 100);
}