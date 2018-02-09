<instrument>
	<!-- layout -->
	<div class="inst_container {opts.name}">
		<div class="header">
			<span>{ opts.name }</span> 
			<form>
				PORT:<br>
				<input class="port {opts.name}" type="text" name="port">
			</form>
		</div>
		<div class="white_key c {opts.name}"></div>
		<div class="black_key cs {opts.name}"></div>
		<div class="white_key d {opts.name}"></div>
		<div class="black_key ds {opts.name}"></div>
		<div class="white_key e {opts.name}"></div>
		<div class="white_key f {opts.name}"></div>
		<div class="black_key fs {opts.name}"></div>
		<div class="white_key g {opts.name}"></div>
		<div class="black_key gs {opts.name}"></div>
		<div class="white_key a {opts.name}"></div>
		<div class="black_key as {opts.name}"></div>
		<div class="white_key b {opts.name}"></div>
		<div class="white_key ch {opts.name}"></div>
	</div>
	<div class="counter {opts.name}"></div>

	<!-- style -->
	<style>
		.inst_container {
			position: relative;
			margin: 10px;
			width: 44vw;
			height: 32vw;
			border-radius: 1vw;
			border: 3px solid;
			background-color: white;
		}

		.header {
			position: absolute;
			width: 40.8vw;
			height: 7.2vw;
			left: 2.1vw;
			top: 2vw;
			background-size: contain;
			background-image: url('/static/instruments/header.png');
		}

		.header span {
			position: absolute;
			font-family: sans-serif;
			color: #10AAAF;
			font-size: 5vh;
			font-weight: bold;
			text-shadow:
			 	-1px -1px 0 #000,  
			  1px -1px 0 #000,
			  -1px 1px 0 #000,
			  1px 1px 0 #000;
			top: 3.6vw;
			left: 2vw;
		}

		.header form {
			position: absolute;
			top: 3vw;
			left: 24vw;
			color: #10AAAF;
			font-family: sans-serif;
			text-shadow:
			 	-1px -1px 0 #000,  
			  1px -1px 0 #000,
			  -1px 1px 0 #000,
			  1px 1px 0 #000;
		}

		.header form input {
			background-color: #333;
			border-left: 2px solid #10AAAF;
			color: #29a329;
		}

		.white_key, .black_key {
			position: absolute;
			top: 9.2vw;
			background-size: cover;
		}

		.white_key {
			width: 5vw;
			height: 21.4vw;
			background-image: url('/static/instruments/white_sprite.png');
			background-position: 0 0;
			z-index: 100;
		}

		.white_key.active {
			background-position: 0 21.4vw;
		}

		.black_key {
			width: 3.2vw;
			height: 12vw;
			background-image: url('/static/instruments/black_sprite.png');
			background-position: 0 0;
			z-index: 200;
		}

		.black_key.active {
			background-position: 0 12vw;
		}

		.c {
			left: 2.5vw;
		}

		.cs {
			left: 5.9vw;
		}

		.d {
			left: 7.5vw;
		}

		.ds {
			left: 10.9vw;
		}

		.e {
			left: 12.5vw;
		}

		.f {
			left: 17.5vw;
		}

		.fs {
			left: 20.9vw;
		}

		.g {
			left: 22.5vw;
		}

		.gs {
			left: 25.9vw;
		}

		.a {
			left: 27.5vw;
		}

		.as {
			left: 30.9vw;
		}

		.b {
			left: 32.5vw;
		}

		.ch {
			left: 37.5vw;
		}

		.counter {
			visibility: hidden;
		}
	</style>

	<!-- script -->
	<script>

		var clickedKey = function(e) {

			// animate keys
			e.classList.add('active');

			document.addEventListener('keyup', function() {
				e.classList.remove('active');
			});

			document.addEventListener('mouseup', function() {
				e.classList.remove('active');
			});

			// gather request info
			classes = e.className.split(" ");
			port_input = document.getElementsByClassName("port " + classes[2])[0];
			port_input = port_input.value.replace(/^\s+|\s+$/g, '');

			// increase counter
			counter = document.getElementsByClassName("counter " + classes[2])[0];	
			count = parseInt(counter.innerHTML) + 1;
			counter.innerHTML = count;

			payload = 
				JSON.stringify({
					name: classes[2],
					key: classes[1],
					port: port_input,
					count: counter.innerHTML
				});	

			fetch('/play/jsonhandler/', {
		    method: 'POST',
		    body: payload,
		    headers: {
		    	"X-CSRFToken": "CSRF_COOKIE",
	        "Content-Type": "application/x-www-form-urlencoded; charset=utf-8"
		    },
		    credentials: 'include',
		    mode: 'cors',
		    cache: 'default',
			});
		}

		this.on('mount', function() {
			// initialize counter
			counter = document.getElementsByClassName(opts.name + " counter")[0];
			counter.innerHTML = 0;

			// allow clicking
			white = document.getElementsByClassName(opts.name + " white_key");
			for (var i=0; i<white.length; i++) {
				white[i].addEventListener('mousedown', function(n) {
					return function() {clickedKey(n)};
				}(white[i]), false);
			}

			black = document.getElementsByClassName(opts.name + " black_key");
			for (var i=0; i<black.length; i++) {
				black[i].addEventListener('mousedown', function(n) {
					return function() {clickedKey(n)};
				}(black[i]), false);
			}

		})

		// allow keypresses
		document.onkeydown = function(event){
			var key = event.which || event.keyCode;
			for (var num in opts.opts) {
				for(var note in opts.opts[num]) {
					element = opts.opts[num]
					if (element[note]==String.fromCharCode(key+32)) {
						classes = element["name"] + " " + note;
						e = document.getElementsByClassName(classes)[0];
						clickedKey(e);
					}
				}
			}
		}

	</script>
</instrument>