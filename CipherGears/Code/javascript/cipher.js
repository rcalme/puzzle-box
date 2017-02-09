// Object that represents a single gear
function Gear(element, spokes, letter) {
    // Available gears and their letter order
    var available = {
        "2-spoke": "OJKWTVCZSBEAFXRYNUHDMGQLPI",
        "3-spoke": "WHZBQROSCNEFUMDIYGPVXTJKAL",
        "4-spoke": "UHMLONTGIAZWKFXQRYVJEPSCBD",
        "5-spoke": "FDQZMPXTJWIKHRGVSEUCANYLBO",
        "6-spoke": "DZNSRKFUMPXCAYBQGVEWOJTLIH"
    };

    // Rotate a gear image
    this.rotateImage = function(teeth) {
        this.angle += ((360/26) * teeth ) % 360;
        this.element.css({
            transform: "rotate(" + this.angle + "deg)"
        });
    };

    // Rotate an array of letters
    this.rotateLetters = function(teeth) {
	var removed = this.letters.splice(teeth);
        this.letters.splice.apply(this.letters, [0,0].concat(removed));
    };

    // Rotate a gear, and the letters that go with
    this.rotate = function(teeth) {
        this.rotateImage(teeth);
        this.rotateLetters(teeth);
    };

    // Find number of teeth that need to shift
    this.lookupDistance = function(letter) {
        var rotationDistance = this.letters.indexOf(letter.toUpperCase());
        return rotationDistance;
    };

    // Set attributes of the Gear object
    this.spokes = spokes;
    this.letters = available[spokes+"-spoke"].split("");
    this.angle = 0;
    this.element = element;
    this.element.attr("src", "image/"+spokes+"-spoke.svg");
    // The middle gear gets rotated an extra half tooth
    if(this.element.hasClass("middle")) {
        this.rotateImage(0.5);
    };

    // Find this key letter on the current gear, rotate that many steps
    this.rotate(this.lookupDistance(letter));
};

// Collection of three gears, and functions to use them
function GearSet(gears) {
    this.gears = gears;

    // Rotate all three gears, in alternating orders
    this.rotateImages = function(teeth) {
        for(var i=0; i<this.gears.length; i++) {
            // Every other gear rotates in opposite directions
            this.gears[i].rotateImage(teeth * Math.pow(-1, i % 2));
        }
    };
    // Rotate all three gears, in alternating orders
    this.rotateLetters = function(teeth) {
        for(var i=0; i<this.gears.length; i++) {
            // Every other gear rotates in opposite directions
            this.gears[i].rotateLetters(teeth * Math.pow(-1, i % 2));
        }
    };
    // Plaintext in, ciphertext out
    this.encrypt = function(plaintext) {
        var ciphertext = "";
        var letters = plaintext.split("");
        var idx = letters.length-1;
        var lastGear = this.getGear((idx % 2) + 2);
        var requiredRotation = lastGear.lookupDistance(letters[idx]) * Math.pow(-1, (idx % 2) + 1);
        for(var i=0; i<letters.length; i++) {
            var inGear = this.getGear((i%2)+2);
            var outGear = this.getGear(1)
            ciphertext += this.translateLetter(inGear, outGear, letters, i, "encrypt");
        }
        // Rotate the images to match the last letter
        this.rotateImages(requiredRotation);
        return this.groupInFours(ciphertext);
    };
    // Ciphertext in, plaintext out
    this.decrypt = function(ciphertext) {
        var plaintext = ""
        var letters = ciphertext.split("");
        var requiredRotation = this.getGear(1).lookupDistance(letters[letters.length-1]);
        for(var i=0; i<letters.length; i++) {
            var inGear = this.getGear(1);
            var outGear = this.getGear((i%2)+2)
            plaintext += this.translateLetter(inGear, outGear, letters, i, "decrypt");
        }
        // Rotate the images to match the last letter
        this.rotateImages(requiredRotation);
        return plaintext;
    };
    // Group in fours for prettyness
    this.groupInFours = function(text) {
        var letters = text.split("");
        var grouped = "";
        for(var i=0; i<letters.length; i++) {
            grouped += letters[i];
            if(i%4 == 3) {
                grouped += " ";
            }
        }
        return grouped;
    }
    // Given an input gear, and an output gear, translates letter to replacement
    this.translateLetter = function(inGear, outGear, letters, pos, mode) {
        // Determine the amount of rotation
        var teeth = inGear.lookupDistance(letters[pos]) * Math.pow(-1, (pos+1)%2);
        if(mode == "decrypt"){
            teeth = Math.abs(teeth);
        }
        // Rotate the set of letters for all gears
        this.rotateLetters(teeth);
        //console.log(letters[pos]+" -> "+outGear.letters[0]+" "+inGear.spokes+"-spoke -> "+outGear.spokes+"-spoke");
        // Read the head letter off the output gear
        return outGear.letters[0];
    }
    // Return the gear object at the index specified (1-based)
    this.getGear = function(gearNumber) {
        return this.gears[gearNumber-1];
    };
};

// Toggle visibility of arrows
function toggleVisibility(messageLength){
    if(messageLength % 2 == 1) {
      $("#arrow2").css('visibility','visible');
      $("#arrow3").css('visibility','hidden');
    }
    else {
      $("#arrow2").css('visibility','hidden');
      $("#arrow3").css('visibility','visible');
    }
};

// Reset (empty) input and output text
function reset() {
    $("#inputBox").val("");
    $("#outputBox").val("");
    $("#arrow1").css('visibility','visible');
    toggleVisibility(1);
}

// Convert input text into output text
function convert(e) {
    // Don't do this if the input was readonly
    if($("#inputBox").prop("readonly")){
        return;
    }
    // Only translate A-Z characters
    var incoming = $(this).val().replace(/[^A-Za-z]/g,'').toUpperCase();
    if(incoming.length) {
        if(mode == ENCRYPT) {
            // Encrypt the string
            $("#outputBox").val(gearSet.encrypt(incoming));
        }
        else {
            // Decrypt the string
            $("#outputBox").val(gearSet.decrypt(incoming));
        }
        // Make the correct output arrow visible
        if(incoming.length > 1){
            toggleVisibility(incoming.length);
        }
    }
}

function setMode(mode){
    // Change the visible mode, rotate the input/output arrows
    if(mode == true) {
        $("#modeLeft").text("Plaintext");
        $("#modeRight").text("Ciphertext");
        $("#arrow1").css({transform: "rotate(180deg)"});
        $("#arrow2").css({transform: "rotate(0deg)"});
        $("#arrow3").css({transform: "rotate(0deg)"});
    } else {
        $("#modeLeft").text("Ciphertext");
        $("#modeRight").text("Plaintext");
        $("#arrow1").css({transform: "rotate(0deg)"});
        $("#arrow2").css({transform: "rotate(180deg)"});
        $("#arrow3").css({transform: "rotate(180deg)"});
    }
}

const ENCRYPT = true;
const DECRYPT = false;

var gearSet;
var mode = ENCRYPT;

// When the page finishes loading, execute this
$(document).ready(function() {
    // Set encrypt mode initially
    setMode(mode);

    // Disable the right-side (output) textarea
    $("#inputBox").prop('disabled', true);
    $("#outputBox").prop('disabled', true);
    reset();

    // When pressing the button to lock in gears, do so.
    $("#setGearsButton").click(function() {
        // All of these must be filled before we continue.
        if( !$("#spoke1").val() ||
            !$("#spoke2").val() ||
            !$("#spoke3").val() ||
            !$("#key1").val() ||
            !$("#key2").val() ||
            !$("#key3").val() ) {
            return;
        }
        var gear1 = new Gear($("#gear-1"), $("#spoke1 option:selected").text(), $("#key1").val());
        var gear2 = new Gear($("#gear-2"), $("#spoke2 option:selected").text(), $("#key2").val());
        var gear3 = new Gear($("#gear-3"), $("#spoke3 option:selected").text(), $("#key3").val());
        // Make a set of gears
        gearSet = new GearSet([gear1, gear2, gear3]);
        // The encrypt/decrypt slider is now accessible
        $("#inputBox").prop('disabled', false);
        reset();
    });

    // When switching between encrpyt and decrypt
    $("label.onoffswitch-label").click(function() {
        mode = !mode;
        // Empty both input and output text boxes
        setMode(mode);
        reset();
    });

    // When the input text box changes
    $("#inputBox").keyup(convert);
});

