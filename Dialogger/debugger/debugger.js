function runScript(dlz){
	console.log(dlz);
}

var globals = {
	x:'32',
	y:'42',
	pi:'3.14159',
	A_cool_key: 'with a cool value',
	Hello_World: 'Goodbye World!',
	coolKey: 'cooler val',
	myVar: 'MyValue',
	xts: 'storage',
	lists: 'lists',
	awdawdddddddddddd: 'setmset',
	user: 'name',
	oh: 'my',
	test: 'h',
	cpp: '++',
	more: 'here',
	key1: 'val1',
	key2: 'val2',
	key3: 'val3',
	key4: 'val4',
	key5: 'val5',
	key6: 'val6',
	key7: 'val7',
	key8: 'val8',
};

var view = {};

$('#debug').html('tests');
$('#debug').html(localStorage['dia_script']);

$(window).resize(function()
{
	$('#global_list').height($(window).innerHeight()-100);
	return true;
});

//console.log(localStorage[localStorage.key(0)]);
$(window).on('keydown', function(event)
{
	if ((event.ctrlKey || event.metaKey) && String.fromCharCode(event.which).toLowerCase() == 'd')
	{
		if($('#memory_profiler').css('display')=='none')
		{
			$('#memory_profiler').css('display','inline');
		}
		else
		{
			$('#memory_profiler').css('display','none');
		}
		return false;
	}
	return true;
});

(function(){
	$('#global_list').height($(window).innerHeight()-100);
	
	for(k in globals)
	{
		view[k] = $('#global_list').append('<li id="'+k+'">'+getText(k)+'<input type="text" placeholder="value"'+'id="value" value="'+globals[k]+'"'+'></input></li>').find('#'+k);
		eventHandler(view[k]);
	}
})();

//Truncates a string once it's length passes 10
function getText(str)
{
	if(str.length>10)
		return str.substring(0,9)+'...';
	else
		return str;
}

//Attaches an event handler to a list item's on change event to update it's corresponding variable value stored in Dialog.global
function eventHandler(obj)
{
	//alert(obj.find('input').attr('value'));
	
	obj.find('input').on('change',_.bind(function(evt){
		globals[this.attr('id')] = $(evt.target).val();
		$('#memory_profiler').css('display','none');
	},obj));
	/*obj.find('input').on('input',function(){
		//alert('hello world!');
		$('#memory_profiler').css('display','none');
	});*/
}
//Adds/Updates a value
function addValue(k,v)
{
	if(view[k])
	{
		alert('updating view');
		view[k].find('input').attr('value',v);
	}
	else
	{
		view[k] = $('#global_list').append('<li id="'+k+'">'+getText(k)+'<input placeholder="value"'+'id="value" value="'+v+'"'+'></input></li>').find('#'+k);
	}
}

function getField(id)
{
	return $('#global_list').find(id);
}