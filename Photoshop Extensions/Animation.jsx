#target photoshop

app.bringToFront();

var doc;
var docName;
//Define pixels as unit of measurement
var defaultRulerUnits = preferences.rulerUnits;
preferences.rulerUnits = Units.PIXELS;

var layerNum;

var FPath;

var layerRef;
var projectionAngle = 0.5;
var transform;
var data = [];

var dlg;

function main(){
    if(app.documents.length === 0) { alert("No Active Document"); reset(); return; }
    
     doc = app.activeDocument;

    if(doc.layerSets.length === 0) { alert("There are no layer groups in the active document"); reset (); return; }

     docName = decodeURI (doc.name);
     
     layerNum = doc.artLayers.length;
     
     layerRef = doc.activeLayer
     
     dlg = new Window("dialog{text:'Animation Exporter',bounds:[100,100,561,269],\
    text:StaticText{bounds:[16,16,444.95,94] , text:'0.5' ,properties:{multiline:false,scrolling:false}},\
    button0:Button{bounds:[17,102,117,122] , text:'Confirm' },\
    button1:Button{bounds:[236,101,336,121] , text:'Cancel' },\
    sliderX:Slider{bounds:[18,138,173,148] , minvalue:1,maxvalue:128,value:40, text:'x: '}\
    sliderY:Slider{bounds:[18,290,173,148] , minvalue:1,maxvalue:128,value:40, text:'y: '}\
    checkbox0:Checkbox{bounds:[190,133,261,154] , text:'Export frames' },\
    dropdown0:DropDownList{bounds:[299,134,443,149],properties:{items:['Select One']}}\
    };");
    
    dlg.sliderX.onChange = function(){
            updateTransform();
    };

    dlg.sliderY.onChange = function(){
            updateTransform();
    };
    //User began export
    dlg.Button0.onClick = function(){
        recurseAnimations (doc);
        
        FPath = Folder.selectDialog("Save Animation file to");

        var fileLineFeed = 'Windows';

        if($.os.search(/window/i) !== -1){
                fileLineFeed = 'Windows';
        }
        else{
                fileLineFeed = 'Macintosh';
        }

         writeFile(FPath,fileLineFeed);
         reset();
         dlg.close();
    };
    //User canceled
    dlg.Button1.onClick = function(){
            reset();
            dlg.close();
    };
    updateTransform ();
    dlg.show();
}

function updateTransform(){
        projectionAngle = Math.atan(dlg.sliderY.value/dlg.sliderX.value);
        transform = [Math.cos(projectionAngle),Math.sin(projectionAngle)];
        dlg.text.text = projectionAngle.toString();
}

function recurseAnimations(currLayers){

    for(var i=0;i<currLayers.layers.length;i++){
            var animation = {};
            
            if(isLayerSet (layer)){
                animation.name = decodeURI(layerRef.name);
                animation.fps = 16;
                animation.frames = [];
                recurseFrames(currLayers.layers[i],animation);
            }
    }
}

function recurseFrames(currLayers,animation){
    var x = 0;
    var y = 0;
    var refFrame = currLayers.layers[0];
    
    //Start at index using the first frame as a reference for generating positions
    for(var i=1;i<currLayers.layers.length;i++){
            layerRef = currLayers.layers[i];
            
            x = layerRef.bounds[0].value-refFrame.bounds[0].value;
            y = layerRef.bounds[1].value-refFrame.bounds[1].value;
            
            var coords = project(x,y);
            
            animation.frames.push({
                x: coords[0],
                y: coords[1]
            });
        
            data.push(animation);
    }
}

function hideLayers(){
        for(var i=0;i<doc.layerSets.length;i++){
            doc.layerSets[i].visible = false;
        }
}

function generateFrameNames(prefix, start, stop, suffix, zeroPad){
        var output = [];
        var frame = '';
        
        if(start < stop){
            for(var i=start;i<=stop;i++){
                  if(typeof zeroPad === 'number'){
                        frame = pad(i.toString(), zeroPad, '0', 1);
                  }
                else{
                    frame = i.toString();
                }
                frame = prefix + frame + suffix;
                output.push(frame);
            }
        }
        else{
            for(var i=start;i>=stop;i--){
                if(typeof zeroPad==='number'){
                      frame = pad(i.toString(), zeroPad, '0',1);
                }
                else{
                      frame = i.toString();
                }
                frame = prefix + frame + suffix;
                output.push(frame);
            }
        }
        return output;
}

function pad(str, len, pad, dir){
    if  (len === undefined) { var len = 0; }
    if  (pad === undefined) { var pad = ' '; }
    if  (dir === undefined) { var dir = 3; }
    
    var padlen = 0;
    
    if(len+1 >= str.length){
        switch(dir){
            case 1:
                str = new Array(len+1-str.length).join(pad) + str;
                break;
            case 3:
                var right = Math.ceil((padlen = len - str.length)/2);
                var left = padlen - right;
                str = new Array(left+1).join(pad)+str+new Array(right+1).join(pad);
                break;
            default:
                str = str + new Array(len+1-str.length).join(pad);
                break;
       }
    }
    return str;
}

function project(x,y){
        var coords = [];
        
        coords[0] = x / (2 * transform[0]) + y / (2 * transform[1]);
        coords[1] = -(x / (2 * transform[0])) + y / (2 * transform[1]);
        
        return coords;
}

function isLayerSet(layer){
        try{
                if(layer.layers.length > 0){
                        return true;
                }
         }
     catch(err){
            return false;
      }
}

function writeFile(path,lineFeed){
        try{
            var f = new File(path + '/' + docName + '.json'); 
            f.remove();
            f.open('a');
            f.lineFeed  = lineFeed;
            f.write(JSON.stringify(data));
            f.close();
        }
        catch(e){
            alert('failed to write file');
        }
}

function reset(){
        //Set preferences back to user's defaults
        preferences.rulerUnits = defaultRulerUnits;
}

main();