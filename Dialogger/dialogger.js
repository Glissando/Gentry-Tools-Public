var fs = null;
addEventListener('app-ready', function(e)
{
	// We're running inside app.js
	fs = require('fs');
	$('#import').hide();
	$('#export').hide();
	$('#export-game').hide();
});

var graph = new joint.dia.Graph();

var defaultLink = new joint.dia.Link(
{
	attrs:
	{
		'.marker-target': { d: 'M 10 0 L 0 5 L 10 10 z', },
		'.link-tools .tool-remove circle, .marker-vertex': { r: 8 },
	},
});
defaultLink.set('smooth', true);

var allowableConnections =
[
	//Text connections
	['dialogue.Text', 'dialogue.Text'],
	['dialogue.Text', 'dialogue.Choice'],
	['dialogue.Text', 'dialogue.Set'],
	['dialogue.Text', 'dialogue.Branch'],
	['dialogue.Text', 'dialogue.Delay'],
	['dialogue.Text', 'dialogue.Link'],
	['dialogue.Text', 'dialogue.Audio'],
	['dialogue.Text', 'dialogue.Play'],
	['dialogue.Text', 'dialogue.Stop'],
	['dialogue.Text', 'dialogue.Sprite'],
	['dialogue.Text', 'dialogue.Show'],
	['dialogue.Text', 'dialogue.Hide'],
	['dialogue.Text', 'dialogue.Tween'],
	//Root connections
	['dialogue.Node', 'dialogue.Text'],
	//['dialogue.Node', 'dialogue.Choice'],
	['dialogue.Node', 'dialogue.Set'],
	['dialogue.Node', 'dialogue.Branch'],
	['dialogue.Node', 'dialogue.Delay'],
	['dialogue.Node', 'dialogue.Link'],
	['dialogue.Node', 'dialogue.Audio'],
	['dialogue.Node', 'dialogue.Play'],
	['dialogue.Node', 'dialogue.Stop'],
	['dialogue.Node', 'dialogue.Sprite'],
	['dialogue.Node', 'dialogue.Show'],
	['dialogue.Node', 'dialogue.Hide'],
	['dialogue.Node', 'dialogue.Tween'],
	//Choice connections
	['dialogue.Choice', 'dialogue.Text'],
	['dialogue.Choice', 'dialogue.Set'],
	['dialogue.Choice', 'dialogue.Branch'],
	['dialogue.Choice', 'dialogue.Delay'],
	['dialogue.Choice', 'dialogue.Link'],
	['dialogue.Choice', 'dialogue.Audio'],
	['dialogue.Choice', 'dialogue.Play'],
	['dialogue.Choice', 'dialogue.Stop'],
	['dialogue.Choice', 'dialogue.Sprite'],
	['dialogue.Choice', 'dialogue.Show'],
	['dialogue.Choice', 'dialogue.Hide'],
	['dialogue.Choice', 'dialogue.Tween'],
	//Set connections
	['dialogue.Set', 'dialogue.Text'],
	['dialogue.Set', 'dialogue.Set'],
	['dialogue.Set', 'dialogue.Branch'],
	['dialogue.Set', 'dialogue.Link'],
	['dialogue.Set', 'dialogue.Audio'],
	['dialogue.Set', 'dialogue.Play'],
	['dialogue.Set', 'dialogue.Stop'],
	['dialogue.Set', 'dialogue.Sprite'],
	['dialogue.Set', 'dialogue.Show'],
	['dialogue.Set', 'dialogue.Hide'],
	['dialogue.Set', 'dialogue.Tween'],
	//Branch connections
	['dialogue.Branch', 'dialogue.Text'],
	['dialogue.Branch', 'dialogue.Set'],
	['dialogue.Branch', 'dialogue.Branch'],
	['dialogue.Branch', 'dialogue.Delay'],
	['dialogue.Branch', 'dialogue.Link'],
	['dialogue.Branch', 'dialogue.Audio'],
	['dialogue.Branch', 'dialogue.Play'],
	['dialogue.Branch', 'dialogue.Stop'],
	['dialogue.Branch', 'dialogue.Sprite'],
	['dialogue.Branch', 'dialogue.Show'],
	['dialogue.Branch', 'dialogue.Hide'],
	['dialogue.Branch', 'dialogue.Tween'],
	//Delay connections
	['dialogue.Delay', 'dialogue.Text'],
	['dialogue.Delay', 'dialogue.Set'],
	['dialogue.Delay', 'dialogue.Branch'],
	['dialogue.Delay', 'dialogue.Link'],
	['dialogue.Delay', 'dialogue.Play'],
	['dialogue.Delay', 'dialogue.Stop'],
	['dialogue.Delay', 'dialogue.Show'],
	['dialogue.Delay', 'dialogue.Hide'],
	['dialogue.Delay', 'dialogue.Tween'],
	//Link connections
	['dialogue.Link', 'dialogue.Text'],
	['dialogue.Link', 'dialogue.Choice'],
	['dialogue.Link', 'dialogue.Branch'],
	['dialogue.Link', 'dialogue.Set'],
	['dialogue.Link', 'dialogue.Play'],
	['dialogue.Link', 'dialogue.Stop'],
	['dialogue.Link', 'dialogue.Show'],
	['dialogue.Link', 'dialogue.Hide'],
	['dialogue.Link', 'dialogue.Tween'],
	//Audio connections
	['dialogue.Audio', 'dialogue.Text'],
	['dialogue.Audio', 'dialogue.Set'],
	['dialogue.Audio', 'dialogue.Branch'],
	['dialogue.Audio', 'dialogue.Delay'],
	['dialogue.Audio', 'dialogue.Link'],
	['dialogue.Audio', 'dialogue.Audio'],
	['dialogue.Audio', 'dialogue.Play'],
	['dialogue.Audio', 'dialogue.Stop'],
	['dialogue.Audio', 'dialogue.Sprite'],
	['dialogue.Audio', 'dialogue.Show'],
	['dialogue.Audio', 'dialogue.Hide'],
	['dialogue.Audio', 'dialogue.Tween'],
	//Play connections
	['dialogue.Play', 'dialogue.Text'],
	['dialogue.Play', 'dialogue.Set'],
	['dialogue.Play', 'dialogue.Branch'],
	['dialogue.Play', 'dialogue.Delay'],
	['dialogue.Play', 'dialogue.Link'],
	['dialogue.Play', 'dialogue.Audio'],
	['dialogue.Play', 'dialogue.Play'],
	['dialogue.Play', 'dialogue.Stop'],
	['dialogue.Play', 'dialogue.Sprite'],
	['dialogue.Play', 'dialogue.Show'],
	['dialogue.Play', 'dialogue.Hide'],
	['dialogue.Play', 'dialogue.Tween'],
	//Stop connections
	['dialogue.Stop', 'dialogue.Text'],
	['dialogue.Stop', 'dialogue.Set'],
	['dialogue.Stop', 'dialogue.Branch'],
	['dialogue.Stop', 'dialogue.Delay'],
	['dialogue.Stop', 'dialogue.Link'],
	['dialogue.Stop', 'dialogue.Audio'],
	['dialogue.Stop', 'dialogue.Play'],
	['dialogue.Stop', 'dialogue.Stop'],
	['dialogue.Stop', 'dialogue.Sprite'],
	['dialogue.Stop', 'dialogue.Show'],
	['dialogue.Stop', 'dialogue.Hide'],
	['dialogue.Stop', 'dialogue.Tween'],
	//Sprite connections
	['dialogue.Sprite', 'dialogue.Text'],
	['dialogue.Sprite', 'dialogue.Set'],
	['dialogue.Sprite', 'dialogue.Branch'],
	['dialogue.Sprite', 'dialogue.Link'],
	['dialogue.Sprite', 'dialogue.Audio'],
	['dialogue.Sprite', 'dialogue.Play'],
	['dialogue.Sprite', 'dialogue.Stop'],
	['dialogue.Sprite', 'dialogue.Sprite'],
	['dialogue.Sprite', 'dialogue.Show'],
	['dialogue.Sprite', 'dialogue.Hide'],
	['dialogue.Sprite', 'dialogue.Tween'],
	//Show connections
	['dialogue.Show', 'dialogue.Text'],
	['dialogue.Show', 'dialogue.Set'],
	['dialogue.Show', 'dialogue.Branch'],
	['dialogue.Show', 'dialogue.Delay'],
	['dialogue.Show', 'dialogue.Link'],
	['dialogue.Show', 'dialogue.Audio'],
	['dialogue.Show', 'dialogue.Play'],
	['dialogue.Show', 'dialogue.Stop'],
	['dialogue.Show', 'dialogue.Sprite'],
	['dialogue.Show', 'dialogue.Show'],
	['dialogue.Show', 'dialogue.Hide'],
	['dialogue.Show', 'dialogue.Tween'],
	//Hide connections
	['dialogue.Hide', 'dialogue.Text'],
	['dialogue.Hide', 'dialogue.Set'],
	['dialogue.Hide', 'dialogue.Branch'],
	['dialogue.Hide', 'dialogue.Delay'],
	['dialogue.Hide', 'dialogue.Link'],
	['dialogue.Hide', 'dialogue.Audio'],
	['dialogue.Hide', 'dialogue.Play'],
	['dialogue.Hide', 'dialogue.Stop'],
	['dialogue.Hide', 'dialogue.Sprite'],
	['dialogue.Hide', 'dialogue.Show'],
	['dialogue.Hide', 'dialogue.Hide'],
	['dialogue.Hide', 'dialogue.Tween'],
	//Tween
	['dialogue.Tween', 'dialogue.Text'],
	['dialogue.Tween', 'dialogue.Set'],
	['dialogue.Tween', 'dialogue.Branch'],
	['dialogue.Tween', 'dialogue.Delay'],
	['dialogue.Tween', 'dialogue.Link'],
	['dialogue.Tween', 'dialogue.Audio'],
	['dialogue.Tween', 'dialogue.Play'],
	['dialogue.Tween', 'dialogue.Stop'],
	['dialogue.Tween', 'dialogue.Sprite'],
	['dialogue.Tween', 'dialogue.Show'],
	['dialogue.Tween', 'dialogue.Hide'],
	['dialogue.Tween', 'dialogue.Tween'],
];

function validateConnection(cellViewS, magnetS, cellViewT, magnetT, end, linkView)
{
	// Prevent loop linking
	if (magnetS == magnetT)
		return false;

	if (cellViewS == cellViewT)
		return false;

	if (magnetT.attributes.magnet.nodeValue !== 'passive') // Can't connect to an output port
		return false;

	var sourceType = cellViewS.model.attributes.type;
	var targetType = cellViewT.model.attributes.type;
	var valid = false;
	for (var i = 0; i < allowableConnections.length; i++)
	{
		var rule = allowableConnections[i];
		if (sourceType == rule[0] && targetType == rule[1])
		{
			valid = true;
			break;
		}
	}
	if (!valid)
		return false;

	var links = graph.getConnectedLinks(cellViewS.model);
	for (var i = 0; i < links.length; i++)
	{
		var link = links[i];
		if (link.attributes.source.id === cellViewS.model.id && link.attributes.source.port === magnetS.attributes.port.nodeValue && link.attributes.target.id)
		{
			var targetCell = graph.getCell(link.attributes.target.id);
			if (targetCell.attributes.type !== targetType)
				return false; // We can only connect to multiple targets of the same type
			if (targetCell == cellViewT.model)
				return false; // Already connected
		} 
	}

	return true;
}

function validateMagnet(cellView, magnet)
{
	if (magnet.getAttribute('magnet') === 'passive')
		return false;

	// If unlimited connections attribute is null, we can only ever connect to one object
	// If it is not null, it is an array of type strings which are allowed to have unlimited connections
	var unlimitedConnections = magnet.getAttribute('unlimitedConnections');
	var links = graph.getConnectedLinks(cellView.model);
	for (var i = 0; i < links.length; i++)
	{
		var link = links[i];
		if (link.attributes.source.id === cellView.model.id && link.attributes.source.port === magnet.attributes.port.nodeValue)
		{
			// This port already has a connection
			if (unlimitedConnections && link.attributes.target.id)
			{
				var targetCell = graph.getCell(link.attributes.target.id);
				if (unlimitedConnections.indexOf(targetCell.attributes.type) !== -1)
					return true; // It's okay because this target type has unlimited connections
			} 
			return false;
		}
	}

	return true;
}

joint.shapes.dialogue = {};
joint.shapes.dialogue.Base = joint.shapes.devs.Model.extend(
{
	defaults: joint.util.deepSupplement
	(
		{
			type: 'dialogue.Base',
			size: { width: 200, height: 64 },
			name: '',
			attrs:
			{
				rect: { stroke: 'none', 'fill-opacity': 0 },
				text: { display: 'none' },
				'.inPorts circle': { magnet: 'passive' },
				'.outPorts circle': { magnet: true, },
			},
		},
		joint.shapes.devs.Model.prototype.defaults
	),
});
//Node definition
joint.shapes.dialogue.BaseView = joint.shapes.devs.ModelView.extend(
{
	template:
	[
		'<div class="node">',
		'<span class="label"></span>',
		'<button class="delete">x</button>',
		'<input type="text" class="name" placeholder="Text" />',
		'</div>',
	].join(''),

	initialize: function()
	{
		_.bindAll(this, 'updateBox');
		joint.shapes.devs.ModelView.prototype.initialize.apply(this, arguments);

		this.$box = $(_.template(this.template)());
		// Prevent paper from handling pointerdown.
		this.$box.find('input').on('mousedown click', function(evt) { evt.stopPropagation(); });

		// This is an example of reacting on the input change and storing the input data in the cell model.
		this.$box.find('input.name').on('change', _.bind(function(evt)
		{
			this.model.set('name', $(evt.target).val());
		}, this));

		this.$box.find('.delete').on('click', _.bind(this.model.remove, this.model));
		// Update the box position whenever the underlying model changes.
		this.model.on('change', this.updateBox, this);
		// Remove the box when the model gets removed from the graph.
		this.model.on('remove', this.removeBox, this);

		this.updateBox();
	},

	render: function()
	{
		joint.shapes.devs.ModelView.prototype.render.apply(this, arguments);
		this.paper.$el.prepend(this.$box);
		this.updateBox();
		return this;
	},

	updateBox: function()
	{
		// Set the position and dimension of the box so that it covers the JointJS element.
		var bbox = this.model.getBBox();
		// Example of updating the HTML with a data stored in the cell model.
		var nameField = this.$box.find('input.name');
		if (!nameField.is(':focus'))
			nameField.val(this.model.get('name'));
		var label = this.$box.find('.label');
		var type = this.model.get('type').slice('dialogue.'.length);
		label.text(type);
		label.attr('class', 'label ' + type);
		this.$box.css({ width: bbox.width, height: bbox.height, left: bbox.x, top: bbox.y, transform: 'rotate(' + (this.model.get('angle') || 0) + 'deg)' });
	},

	removeBox: function(evt)
	{
		this.$box.remove();
	},
});

joint.shapes.dialogue.Node = joint.shapes.devs.Model.extend(
{
	defaults: joint.util.deepSupplement
	(
		{
			type: 'dialogue.Node',
			inPorts: ['input'],
			outPorts: ['output'],
			attrs:
			{
				'.outPorts circle': { unlimitedConnections: ['dialogue.Choice'], }
			},
		},
		joint.shapes.dialogue.Base.prototype.defaults
	),
});
joint.shapes.dialogue.NodeView = joint.shapes.dialogue.BaseView;

joint.shapes.dialogue.Text = joint.shapes.devs.Model.extend(
{
	defaults: joint.util.deepSupplement
	(
		{
			type: 'dialogue.Text',
			inPorts: ['input'],
			outPorts: ['output'],
			size: { width: 200, height: 100, },
			attrs:
			{
				'.outPorts circle': { unlimitedConnections: ['dialogue.Choice'], }
			},
			value: '',
		},
		joint.shapes.dialogue.Base.prototype.defaults
	),
});
joint.shapes.dialogue.TextView = joint.shapes.dialogue.BaseView.extend(
{
	template:
	[
		'<div class="node">',
		'<span class="label"></span>',
		'<button class="delete">x</button>',
		'<input type="text" class="name" placeholder="Name" />',
		'<input type="text" class="value" placeholder="Text" />',
		'</div>',
	].join(''),

	initialize: function()
	{
		joint.shapes.dialogue.BaseView.prototype.initialize.apply(this, arguments);
		this.$box.find('input.value').on('change', _.bind(function(evt)
		{
			this.model.set('value', $(evt.target).val());
		}, this));
	},

	updateBox: function()
	{
		joint.shapes.dialogue.BaseView.prototype.updateBox.apply(this, arguments);
		var field = this.$box.find('input.value');
		if (!field.is(':focus'))
			field.val(this.model.get('value'));
	},
});

joint.shapes.dialogue.Choice = joint.shapes.devs.Model.extend(
{
	defaults: joint.util.deepSupplement
	(
		{
			type: 'dialogue.Choice',
			inPorts: ['input'],
			outPorts: ['output'],
		},
		joint.shapes.dialogue.Base.prototype.defaults
	),
});

joint.shapes.dialogue.ChoiceView = joint.shapes.dialogue.BaseView;
//Delay definition
joint.shapes.dialogue.Delay = joint.shapes.devs.Model.extend(
{
	defaults: joint.util.deepSupplement
	(
		{
			type: 'dialogue.Delay',
			inPorts: ['input'],
			outPorts: ['output'],
		},
		joint.shapes.dialogue.Base.prototype.defaults
	),
});

joint.shapes.dialogue.DelayView = joint.shapes.dialogue.BaseView;
//Link definition
joint.shapes.dialogue.Link = joint.shapes.devs.Model.extend(
{
	defaults: joint.util.deepSupplement
	(
		{
			type: 'dialogue.Link',
			inPorts: ['input'],
			outPorts: ['output'],
			attrs:
			{
				'.outPorts circle': { unlimitedConnections: ['dialogue.Choice'], }
			},
		},
		joint.shapes.dialogue.Base.prototype.defaults
	),
});

joint.shapes.dialogue.LinkView = joint.shapes.dialogue.BaseView;
//Branch definition
joint.shapes.dialogue.Branch = joint.shapes.devs.Model.extend(
{
	defaults: joint.util.deepSupplement
	(
		{
			type: 'dialogue.Branch',
			size: { width: 200, height: 100, },
			inPorts: ['input'],
			outPorts: ['output0'],
			values: [],
		},
		joint.shapes.dialogue.Base.prototype.defaults
	),
});
//Branch definition
joint.shapes.dialogue.BranchView = joint.shapes.dialogue.BaseView.extend(
{
	template:
	[
		'<div class="node">',
		'<span class="label"></span>',
		'<button class="delete">x</button>',
		'<button class="add">+</button>',
		'<button class="remove">-</button>',
		'<input type="text" class="name" placeholder="Variable" />',
		'<input type="text" value="Default" readonly/>',
		'</div>',
	].join(''),

	initialize: function()
	{
		joint.shapes.dialogue.BaseView.prototype.initialize.apply(this, arguments);
		this.$box.find('.add').on('click', _.bind(this.addPort, this));
		this.$box.find('.remove').on('click', _.bind(this.removePort, this));
	},

	removePort: function()
	{
		if (this.model.get('outPorts').length > 1)
		{
			var outPorts = this.model.get('outPorts').slice(0);
			outPorts.pop();
			this.model.set('outPorts', outPorts);
			var values = this.model.get('values').slice(0);
			values.pop();
			this.model.set('values', values);
			this.updateSize();
		}
	},

	addPort: function()
	{
		var outPorts = this.model.get('outPorts').slice(0);
		outPorts.push('output' + outPorts.length.toString());
		this.model.set('outPorts', outPorts);
		var values = this.model.get('values').slice(0);
		values.push(null);
		this.model.set('values', values);
		this.updateSize();
	},

	updateBox: function()
	{
		joint.shapes.dialogue.BaseView.prototype.updateBox.apply(this, arguments);
		var values = this.model.get('values');
		var valueFields = this.$box.find('input.value');

		// Add value fields if necessary
		for (var i = valueFields.length; i < values.length; i++)
		{
			// Prevent paper from handling pointerdown.
			var field = $('<input type="text" class="value" />');
			field.attr('placeholder', 'Value ' + (i + 1).toString());
			field.attr('index', i);
			this.$box.append(field);
			field.on('mousedown click', function(evt) { evt.stopPropagation(); });

			// This is an example of reacting on the input change and storing the input data in the cell model.
			field.on('change', _.bind(function(evt)
			{
				var values = this.model.get('values').slice(0);
				values[$(evt.target).attr('index')] = $(evt.target).val();
				this.model.set('values', values);
			}, this));
		}

		// Remove value fields if necessary
		for (var i = values.length; i < valueFields.length; i++)
			$(valueFields[i]).remove();

		// Update value fields
		valueFields = this.$box.find('input.value');
		for (var i = 0; i < valueFields.length; i++)
		{
			var field = $(valueFields[i]);
			if (!field.is(':focus'))
				field.val(values[i]);
		}
	},

	updateSize: function()
	{
		var textField = this.$box.find('input.name');
		var height = textField.outerHeight(true);
		this.model.set('size', { width: 200, height: 100 + Math.max(0, (this.model.get('outPorts').length - 1) * height) });
	},
});

joint.shapes.dialogue.Set = joint.shapes.devs.Model.extend(
{
	defaults: joint.util.deepSupplement
	(
		{
			type: 'dialogue.Set',
			inPorts: ['input'],
			outPorts: ['output'],
			size: { width: 200, height: 100, },
			value: '',
		},
		joint.shapes.dialogue.Base.prototype.defaults
	),
});

joint.shapes.dialogue.SetView = joint.shapes.dialogue.BaseView.extend(
{
	template:
	[
		'<div class="node">',
		'<span class="label"></span>',
		'<button class="delete">x</button>',
		'<input type="text" class="name" placeholder="Variable" />',
		'<input type="text" class="value" placeholder="Value" />',
		'</div>',
	].join(''),

	initialize: function()
	{
		joint.shapes.dialogue.BaseView.prototype.initialize.apply(this, arguments);
		this.$box.find('input.value').on('change', _.bind(function(evt)
		{
			this.model.set('value', $(evt.target).val());
		}, this));
	},

	updateBox: function()
	{
		joint.shapes.dialogue.BaseView.prototype.updateBox.apply(this, arguments);
		var field = this.$box.find('input.value');
		if (!field.is(':focus'))
			field.val(this.model.get('value'));
	},
});
//Audio definition
joint.shapes.dialogue.AudioView = joint.shapes.dialogue.BaseView.extend(
{
	template:
	[
		'<div class="audio">',
		'<span class="label"></span>',
		'<button class="delete">x</button>',
		'<input type="text" class="name" placeholder="Name" />',
		'<input type="text" class="value" placeholder="Sound" />',
		'</div>',
	].join(''),

	initialize: function()
	{
		joint.shapes.dialogue.BaseView.prototype.initialize.apply(this, arguments);
		this.$box.find('input.value').on('change', _.bind(function(evt)
		{
			this.model.set('value', $(evt.target).val());
		}, this));
	},

	updateBox: function()
	{
		joint.shapes.dialogue.BaseView.prototype.updateBox.apply(this, arguments);
		var field = this.$box.find('input.value');
		if (!field.is(':focus'))
			field.val(this.model.get('value'));
	},
});

joint.shapes.dialogue.Audio = joint.shapes.devs.Model.extend(
{
	defaults: joint.util.deepSupplement
	(
		{
			type: 'dialogue.Audio',
			inPorts: ['input'],
			outPorts: ['output'],
			size: { width: 150, height: 100, },
			value: '',
		},
		joint.shapes.dialogue.Base.prototype.defaults
	),
});

//Play definition
joint.shapes.dialogue.PlayView = joint.shapes.dialogue.BaseView.extend(
{
	template:
	[
		'<div class="audio">',
		'<span class="label"></span>',
		'<button class="delete">x</button>',
		'<input type="text" class="name" placeholder="Name" />',
		'<input type="text" class="value" placeholder="Loop" />',
		'</div>',
	].join(''),

	initialize: function()
	{
		joint.shapes.dialogue.BaseView.prototype.initialize.apply(this, arguments);
		this.$box.find('input.value').on('change', _.bind(function(evt)
		{
			this.model.set('value', $(evt.target).val());
		}, this));
	},

	updateBox: function()
	{
		joint.shapes.dialogue.BaseView.prototype.updateBox.apply(this, arguments);
		var field = this.$box.find('input.value');
		if (!field.is(':focus'))
			field.val(this.model.get('value'));
	},
});

joint.shapes.dialogue.Play = joint.shapes.devs.Model.extend(
{
	defaults: joint.util.deepSupplement
	(
		{
			type: 'dialogue.Play',
			inPorts: ['input'],
			outPorts: ['output'],
			size: { width: 150, height: 100, },
			value: '',
		},
		joint.shapes.dialogue.Base.prototype.defaults
	),
});

//Stop definition
joint.shapes.dialogue.StopView = joint.shapes.dialogue.BaseView.extend(
{
	template:
	[
		'<div class="audio">',
		'<span class="label"></span>',
		'<button class="delete">x</button>',
		'<input type="text" class="name" placeholder="Name" />',
		'</div>',
	].join(''),

	initialize: function()
	{
		joint.shapes.dialogue.BaseView.prototype.initialize.apply(this, arguments);
		this.$box.find('input.value').on('change', _.bind(function(evt)
		{
			this.model.set('value', $(evt.target).val());
		}, this));
	},

	updateBox: function()
	{
		joint.shapes.dialogue.BaseView.prototype.updateBox.apply(this, arguments);
		var field = this.$box.find('input.value');
		if (!field.is(':focus'))
			field.val(this.model.get('value'));
	},
});

joint.shapes.dialogue.Stop = joint.shapes.devs.Model.extend(
{
	defaults: joint.util.deepSupplement
	(
		{
			type: 'dialogue.Stop',
			inPorts: ['input'],
			outPorts: ['output'],
			size: { width: 150, height: 63, },
			value: '',
		},
		joint.shapes.dialogue.Base.prototype.defaults
	),
});

//Sprite definition
joint.shapes.dialogue.SpriteView = joint.shapes.dialogue.BaseView.extend(
{
	template:
	[
		'<div class="sprite">',
		'<span class="label"></span>',
		'<button class="delete">x</button>',
		'<input type="text" class="name" placeholder="Name" />',
		'<input type="text" class="texture" placeholder="Texture" />',
		'<input type="text" class="frame" placeholder="Frame" />',
		'<input type="text" class="z" placeholder="z" />',
		'</div>',
	].join(''),

	initialize: function()
	{
		joint.shapes.dialogue.BaseView.prototype.initialize.apply(this, arguments);
		this.$box.find('input.texture').on('change', _.bind(function(evt)
		{
			this.model.set('texture', $(evt.target).val());
			console.log(this.model.get('texture'));
		}, this));
		this.$box.find('input.frame').on('change', _.bind(function(evt)
		{
			this.model.set('frame', $(evt.target).val());
		}, this));
		this.$box.find('input.z').on('change', _.bind(function(evt)
		{
			this.model.set('depth', $(evt.target).val());
		}, this));
	},

	updateBox: function()
	{
		joint.shapes.dialogue.BaseView.prototype.updateBox.apply(this, arguments);
		var fieldt = this.$box.find('input.texture');
		var fieldf = this.$box.find('input.frame');
		var fieldz = this.$box.find('input.z');
		if (!fieldt.is(':focus'))
			fieldt.val(this.model.get('texture'));
		if (!fieldf.is(':focus'))
			fieldf.val(this.model.get('frame'));
		if (!fieldz.is(':focus'))
			fieldz.val(this.model.get('depth'));
	},
});

joint.shapes.dialogue.Sprite = joint.shapes.devs.Model.extend(
{
	defaults: joint.util.deepSupplement
	(
		{
			type: 'dialogue.Sprite',
			inPorts: ['input'],
			outPorts: ['output'],
			size: { width: 150, height: 160, },
			value: '',
		},
		joint.shapes.dialogue.Base.prototype.defaults
	),
});

//Show definition
joint.shapes.dialogue.ShowView = joint.shapes.dialogue.BaseView.extend(
{
	template:
	[
		'<div class="sprite">',
		'<span class="label"></span>',
		'<button class="delete">x</button>',
		'<input type="text" class="name" placeholder="Name" />',
		'<input type="text" class="frame" placeholder="Frame" />',
		'<input type="text" class="x" placeholder="x" />',
		'<input type="text" class="y" placeholder="y" />',
		'</div>',
	].join(''),

	initialize: function()
	{
		joint.shapes.dialogue.BaseView.prototype.initialize.apply(this, arguments);
		this.$box.find('input.frame').on('change', _.bind(function(evt)
		{
			this.model.set('frame', $(evt.target).val());
		}, this));
		this.$box.find('input.x').on('change', _.bind(function(evt)
		{
			this.model.set('x', $(evt.target).val());
			console.log('x set to ' + this.model.get('x'));
		}, this));
		this.$box.find('input.y').on('change', _.bind(function(evt)
		{
			this.model.set('y', $(evt.target).val());
			console.log('y set to ' + this.model.get('y'));
		}, this));
	},

	updateBox: function()
	{
		joint.shapes.dialogue.BaseView.prototype.updateBox.apply(this, arguments);
		var field = this.$box.find('input.value');
		var field1 = this.$box.find('input.frame');
		var fieldx = this.$box.find('input.x');
		var fieldy = this.$box.find('input.y');
		if (!field.is(':focus'))
		{
			field.val(this.model.get('value'));
			field1.val(this.model.get('frame'));
			fieldx.val(this.model.get('x'));
			fieldy.val(this.model.get('y'));
		}
	},
});

//Show definition
joint.shapes.dialogue.Show = joint.shapes.devs.Model.extend(
{
	defaults: joint.util.deepSupplement
	(
		{
			type: 'dialogue.Show',
			inPorts: ['input'],
			outPorts: ['output'],
			size: { width: 150, height: 130, },
			value: '',
		},
		joint.shapes.dialogue.Base.prototype.defaults
	),
});

//Hide definition
joint.shapes.dialogue.HideView = joint.shapes.dialogue.BaseView.extend(
{
	template:
	[
		'<div class="sprite">',
		'<span class="label"></span>',
		'<button class="delete">x</button>',
		'<input type="text" class="name" placeholder="Name" />',
		'</div>',
	].join(''),

	initialize: function()
	{
		joint.shapes.dialogue.BaseView.prototype.initialize.apply(this, arguments);
		this.$box.find('input.value').on('change', _.bind(function(evt)
		{
			this.model.set('value', $(evt.target).val());
		}, this));
	},

	updateBox: function()
	{
		joint.shapes.dialogue.BaseView.prototype.updateBox.apply(this, arguments);
		var field = this.$box.find('input.value');
		if (!field.is(':focus'))
			field.val(this.model.get('value'));
	},
});

joint.shapes.dialogue.Hide = joint.shapes.devs.Model.extend(
{
	defaults: joint.util.deepSupplement
	(
		{
			type: 'dialogue.Hide',
			inPorts: ['input'],
			outPorts: ['output'],
			size: { width: 150, height: 65, },
			value: '',
		},
		joint.shapes.dialogue.Base.prototype.defaults
	),
});

//Tween definition
joint.shapes.dialogue.TweenView = joint.shapes.dialogue.BaseView.extend(
{
	template:
	[
		'<div class="sprite">',
		'<span class="label"></span>',
		'<button class="delete">x</button>',
		'<input type="text" class="name" placeholder="Name" />',
		'<input type="text" class="property" placeholder="Property" />',
		'<input type="text" class="ease" placeholder="Interpolation" />',
		'<input type="text" class="duration" placeholder="Duration" />',
		'<input type="text" class="to" placeholder="To" />',
		'<input type="text" class="from" placeholder="From" />',
		
		'</div>',
	].join(''),

	initialize: function()
	{
		joint.shapes.dialogue.BaseView.prototype.initialize.apply(this, arguments);
		this.$box.find('input.property').on('change', _.bind(function(evt)
		{
			this.model.set('property', $(evt.target).val());
		}, this));
		this.$box.find('input.ease').on('change', _.bind(function(evt)
		{
			this.model.set('ease', $(evt.target).val());
		}, this));
		this.$box.find('input.duration').on('change', _.bind(function(evt)
		{
			this.model.set('duration', $(evt.target).val());
		}, this));
		this.$box.find('input.to').on('change', _.bind(function(evt)
		{
			this.model.set('to', $(evt.target).val());
		}, this));
		this.$box.find('input.from').on('change', _.bind(function(evt)
		{
			this.model.set('from', $(evt.target).val());
		}, this));
	},

	updateBox: function()
	{
		joint.shapes.dialogue.BaseView.prototype.updateBox.apply(this, arguments);
		var fieldp = this.$box.find('input.property');
		var fielde = this.$box.find('input.ease');
		var fieldd = this.$box.find('input.duration');
		var fieldt = this.$box.find('input.to');
		var fieldf = this.$box.find('input.from');
		if (!fieldp.is(':focus'))
			fieldp.val(this.model.get('property'));
		if (!fielde.is(':focus'))
			fielde.val(this.model.get('ease'));
		if (!fieldd.is(':focus'))
			fieldd.val(this.model.get('duration'));
		if (!fieldt.is(':focus'))
			fieldt.val(this.model.get('to'));
		if (!fieldf.is(':focus'))
			fieldf.val(this.model.get('from'));
	},
});

joint.shapes.dialogue.Tween = joint.shapes.devs.Model.extend(
{
	defaults: joint.util.deepSupplement
	(
		{
			type: 'dialogue.Tween',
			inPorts: ['input'],
			outPorts: ['output'],
			size: { width: 150, height: 190, },
			value: '',
		},
		joint.shapes.dialogue.Base.prototype.defaults
	),
});

function gameData()
{
	var cells = graph.toJSON().cells;
	var nodesByID = {};
	var cellsByID = {};
	var nodes = [];
	for (var i = 0; i < cells.length; i++)
	{
		var cell = cells[i];
		if (cell.type != 'link')
		{
			var node =
			{
				type: cell.type.slice('dialogue.'.length),
				id: cell.id,
			};
			if (node.type == 'Branch')
			{
				node.variable = cell.name;
				node.branches = {};
				for (var j = 0; j < cell.values.length; j++)
				{
					var branch = cell.values[j];
					node.branches[branch] = null;
				}
			}
			else if (node.type == 'Set')
			{
				node.variable = cell.name;
				node.value = cell.value;
				node.next = null;
			}
			else if (node.type == 'Text')
			{
				node.name = cell.name;
				node.value = cell.value;
			}
			else if (node.type == 'Node')
			{
				node.type = 'Root';
				node.name = cell.name;
			}
			else if (node.type == 'Audio')
			{
				node.name = cell.name;
				node.sound = cell.value;
			}
			else if (node.type == 'Play')
			{
				node.name = cell.name;
				node.value = cell.value;
			}
			else if (node.type == 'Sprite')
			{
				node.name = cell.name;
				node.texture = cell.texture;
				node.frame = cell.frame;
				node.z = cell.depth;
			}
			else if (node.type == 'Show')
			{
				node.name = cell.name;
				node.frame = cell.frame;
				node.x = cell.x;
				node.y = cell.y;
			}
			else if (node.type == 'Hide')
			{
				node.name = cell.name;
			}
			else if (node.type == 'Tween')
			{
				node.name = cell.name;
				node.property = cell.property;
				node.to = cell.to;
				node.from = cell.from;
				node.duration = cell.duration;
				node.ease = cell.ease;
			}
			else
			{
				node.name = cell.name;
				node.next = null;
			}
			nodes.push(node);
			nodesByID[cell.id] = node;
			cellsByID[cell.id] = cell;
		}
	}
	for (var i = 0; i < cells.length; i++)
	{
		var cell = cells[i];
		if (cell.type == 'link')
		{
			var source = nodesByID[cell.source.id];
			var target = cell.target ? nodesByID[cell.target.id] : null;
			if (source)
			{
				if (source.type == 'Branch')
				{
					var portNumber = parseInt(cell.source.port.slice('output'.length));
					var value;
					if (portNumber == 0)
						value = '_default';
					else
					{
						var sourceCell = cellsByID[source.id];
						value = sourceCell.values[portNumber - 1];
					}
					source.branches[value] = target ? target.id : null;
				}
				else if ((source.type == 'Text' || source.type == 'Node') && target && target.type == 'Choice')
				{
					if (!source.choices)
					{
						source.choices = [];
						delete source.next;
					}
					source.choices.push(target.id);
				}
				else
					source.next = target ? target.id : null;
			}
		}
	}
	return nodes;
}

// Menu actions

var filename = null;
var defaultFilename = 'dialogue.dl';

function flash(text)
{
	var $flash = $('#flash');
	$flash.text(text);
	$flash.stop(true, true);
	$flash.show();
	$flash.css('opacity', 1.0);
	$flash.fadeOut({ duration: 1500 });
}

function offerDownload(name, data)
{
	var a = $('<a>');
	a.attr('download', name);
	a.attr('href', 'data:application/json,' + encodeURIComponent(JSON.stringify(data)));
	a.attr('target', '_blank');
	a.hide();
	$('body').append(a);
	a[0].click();
	a.remove();
}

function promptFilename(callback)
{
	if (fs)
	{
		filename = null;
		window.frame.openDialog(
		{
			type: 'save',
		}, function(err, files)
		{
			if (!err && files.length == 1)
			{
				filename = files[0];
				callback(filename);
			}
		});
	}
	else
	{
		filename = prompt('Filename', defaultFilename);
		callback(filename);
	}
}

function applyTextFields()
{
	$('input[type=text]').blur();
}

function save()
{
	applyTextFields();
	if (!filename)
		promptFilename(doSave);
	else
		doSave();
}

function doSave()
{
	if (filename)
	{
		if (fs)
		{
			fs.writeFileSync(filename, JSON.stringify(graph), 'utf8');
			fs.writeFileSync(gameFilenameFromNormalFilename(filename), JSON.stringify(gameData()), 'utf8');
		}
		else
		{
			if (!localStorage[filename])
				addFileEntry(filename);
			localStorage[filename] = JSON.stringify(graph);
		}
		flash('Saved ' + filename);
	}
}

function load()
{
	if (fs)
	{
		window.frame.openDialog(
		{
			type: 'open',
			multiSelect: false,
		}, function(err, files)
		{
			if (!err && files.length == 1)
			{
				graph.clear();
				filename = files[0];
				graph.fromJSON(JSON.parse(fs.readFileSync(filename, 'utf8')));
			}
		});
	}
	else
		$('#menu').show();
}

function exportFile()
{
	if (!fs)
	{
		applyTextFields();
		offerDownload(filename ? filename : defaultFilename, graph);
	}
}

function debug()
{
	var dlz = JSON.stringify(gameData());
	localStorage['dia_script'] = dlz;
	var w = window.open('Debugger.html','Debugger','width 400 height 200');
	console.log('Node count: ' + gameData().length);
	memoryLeft();
}

function memoryLeft()
{
	var allocated = 5;
    var total = 0;
    for(var x in localStorage){  
        var amount = (localStorage[x].length * 2) / 1024 / 1024;  
        total += amount;  
    }
    var remaining = allocated - total;
    console.log( "Used: " + total + " MB");
    console.log( "Remaining: " + remaining + " MB");
}

function gameFilenameFromNormalFilename(f)
{
	return f.substring(0, f.length - 2) + 'dlz';
}

function exportGameFile()
{
	if (!fs)
	{
		applyTextFields();
		offerDownload(gameFilenameFromNormalFilename(filename ? filename : defaultFilename), gameData());
	}
}

function importFile()
{
	if (!fs)
		$('#file').click();
}

function add(constructor)
{
	return function()
	{
		var position = $('#cmroot').position();
		var container = $('#container')[0];
		var element = new constructor(
		{
			position: { x: position.left + container.scrollLeft, y: position.top + container.scrollTop },
		});
		graph.addCells([element]);
	};
}

function clear()
{
	graph.clear();
	filename = null;
}

// Browser stuff
var dimensions = {x:8000,y:6000};
var paper = new joint.dia.Paper(
{
	el: $('#paper'),
	width: dimensions.x,
	height: dimensions.y,
	model: graph,
	gridSize: 1,
	defaultLink: defaultLink,
	validateConnection: validateConnection,
	validateMagnet: validateMagnet,
	// Enable link snapping within 75px lookup radius
	snapLinks: { radius: 75 }
});

navigator.browser = (function(){
    var ua= navigator.userAgent, tem,
    M= ua.match(/(opera|chrome|safari|firefox|msie|trident(?=\/))\/?\s*(\d+)/i) || [];
    if(/trident/i.test(M[1])){
        tem=  /\brv[ :]+(\d+)/g.exec(ua) || [];
        return 'IE '+(tem[1] || '');
    }
    if(M[1]=== 'Chrome'){
        tem= ua.match(/\b(OPR|Edge)\/(\d+)/);
        if(tem!= null) return tem.slice(1).join(' ').replace('OPR', 'Opera');
    }
    M= M[2]? [M[1], M[2]]: [navigator.appName, navigator.appVersion, '-?'];
    if((tem= ua.match(/version\/(\d+)/i))!= null) M.splice(1, 1, tem[1]);
    return M.join(' ');
})();

var panning = false;
var mousePosition = { x: 0, y: 0 };
paper.on('blank:pointerdown', function(e, x, y)
{
	panning = true;
	mousePosition.x = e.pageX;
	mousePosition.y = e.pageY;
	$('body').css('cursor', 'move');
	applyTextFields();
});
paper.on('cell:pointerdown', function(e, x, y)
{
	applyTextFields();
});

var isFirefox = !(!!(~navigator.browser.indexOf('Firefox')));
if(isFirefox)
{
	//We're not in firefox so let's enable infinite scrolling.
	$('#container').scroll(function () 
	{
		var $this = $(this);
		if($this.scrollTop()>dimensions.y-$(document).height()-300){
			dimensions.y+=1000;
			paper.setDimensions(dimensions.x,dimensions.y);
		}
		if($this.scrollLeft()>dimensions.x-$(document).width()-300){
			dimensions.x+=1000;
			paper.setDimensions(dimensions.x,dimensions.y);
		}
	});
}
else
{
	if (document.cookie.replace(/(?:(?:^|.*;\s*)feature_alert\s*\=\s*([^;]*).*$)|^.*$/, "$1") !== 'true')
	{
		alert('Infinite scrolling is not supported in Firefox.');
	}
	document.cookie = 'feature_alert=true';
}

$('#container').mousemove(function(e)
{
	if (panning)
	{
		var $this = $(this);
		$this.scrollLeft($this.scrollLeft() + mousePosition.x - e.pageX);
		$this.scrollTop($this.scrollTop() + mousePosition.y - e.pageY);
		if(isFirefox)
		{
			if($this.scrollTop()>dimensions.y-$(document).height()-300)
			{
				dimensions.y+=1000;
				paper.setDimensions(dimensions.x,dimensions.y);
			}
			if($this.scrollLeft()>dimensions.x-$(document).width()-300)
			{
				dimensions.x+=1000;
				paper.setDimensions(dimensions.x,dimensions.y);
			}
		}
		mousePosition.x = e.pageX;
		mousePosition.y = e.pageY;
	}
});

$('#container').mouseup(function (e)
{
	panning = false;
	$('body').css('cursor', 'default');
});

function handleFiles(files)
{
	filename = files[0].name;
	var fileReader = new FileReader();
	fileReader.onload = function(e)
	{
		graph.clear();
		graph.fromJSON(JSON.parse(e.target.result));
	};
	fileReader.readAsText(files[0]);
}

$('#file').on('change', function()
{
	handleFiles(this.files);
});

$('body').on('dragenter', function(e)
{
	e.stopPropagation();
	e.preventDefault();
});

$('body').on('dragexit', function(e)
{
	e.stopPropagation();
	e.preventDefault();
});

$('body').on('dragover', function(e)
{
	e.stopPropagation();
	e.preventDefault();
});

$('body').on('drop', function(e)
{
	e.stopPropagation();
	e.preventDefault();
	handleFiles(e.originalEvent.dataTransfer.files);
});

$(window).on('keydown', function(event)
{
	// Catch Ctrl-S or key code 19 on Mac (Cmd-S)
	if (((event.ctrlKey || event.metaKey) && String.fromCharCode(event.which).toLowerCase() == 's') || event.which == 19)
	{
		event.stopPropagation();
		event.preventDefault();
		save();
		return false;
	}
	else if ((event.ctrlKey || event.metaKey) && String.fromCharCode(event.which).toLowerCase() == 'o')
	{
		event.stopPropagation();
		event.preventDefault();
		load();
		return false;
	}
	else if ((event.ctrlKey || event.metaKey) && String.fromCharCode(event.which).toLowerCase() == 'e')
	{
		event.stopPropagation();
		event.preventDefault();
		exportFile();
		return false;
	}
	else if ((event.ctrlKey || event.metaKey) && String.fromCharCode(event.which).toLowerCase() == 'd')
	{
		event.stopPropagation();
		event.preventDefault();
		debug();
		return false;
	}
	return true;
});

$(window).resize(function()
{
	applyTextFields();
	var $window = $(window);
	var $container = $('#container');
		$container.height($window.innerHeight());
		$container.width($window.innerWidth());
		var $menu = $('#menu');
		$menu.css('top', Math.max(0, (($window.height() - $menu.outerHeight()) / 2)) + 'px');
		$menu.css('left', Math.max(0, (($window.width() - $menu.outerWidth()) / 2)) + 'px');
		return this;
});

function addFileEntry(name)
{
	if(name!=='dia_script')
	{
	var entry = $('<div>');
	entry.text(name);
	var deleteButton = $('<button class="delete">-</button>');
	entry.append(deleteButton);
	$('#menu').append(entry);

	deleteButton.on('click', function(event)
	{
		localStorage.removeItem(name);
		entry.remove();
		event.stopPropagation();
	});

	entry.on('click', function(event)
	{
		graph.clear();
		graph.fromJSON(JSON.parse(localStorage[name]));
		filename = name;
		$('#menu').hide();
	});
	}
}

(function()
{
	for (var i = 0; i < localStorage.length; i++)
		addFileEntry(localStorage.key(i));
})();

$('#menu button.close').click(function()
{
	$('#menu').hide();
});

$(window).trigger('resize');

$('#paper').contextmenu(
{
	width: 150,
	items:
	[
		{ text: 'Text', alias: '1-1', action: add(joint.shapes.dialogue.Text) },		
		{ text: 'Root', alias: '1-2', action: add(joint.shapes.dialogue.Node) },
		{ type: 'group', alias: '1-3', width: 100, text: 'Utility', items:[
			{ text: 'Choice', alias: '3-1', action: add(joint.shapes.dialogue.Choice) },
			{ text: 'Branch', alias: '3-2', action: add(joint.shapes.dialogue.Branch) },
			{ text: 'Set', alias: '3-3', action: add(joint.shapes.dialogue.Set) },
			{ text: 'Delay', alias: '3-4', action: add(joint.shapes.dialogue.Delay) },
			{ text: 'Link', alias: '3-5', action: add(joint.shapes.dialogue.Link) },
		]},
		{type: 'group', alias: '1-4', width: 100, text: 'Audio', items:[
			{ text: 'AudioClip', alias: '4-1', action: add(joint.shapes.dialogue.Audio) },
			{ text: 'Play', alias: '4-2', action: add(joint.shapes.dialogue.Play) },
			{ text: 'Stop', alias: '4-3', action: add(joint.shapes.dialogue.Stop) },
		]},
		{ type: 'group', alias: '1-5', width: 100, text:'Spritekit',items:[
			{ text:'Sprite', alias:'5-1', action: add(joint.shapes.dialogue.Sprite) },
			{ text:'Show', alias:'5-2', action: add(joint.shapes.dialogue.Show) },
			{ text:'Hide', alias:'5-3', action: add(joint.shapes.dialogue.Hide) },
			{ text:'Tween', alias:'5-4', action: add(joint.shapes.dialogue.Tween) },
		]},
		{ type: 'splitLine' },
		{ text: 'Save', alias: '2-1', action: save },
		{ text: 'Load', alias: '2-2', action: load },
		{ text: 'Import', id: 'import', alias: '2-3', action: importFile },
		{ text: 'New', alias: '2-4', action: clear },
		{ text: 'Export', id: 'export', alias: '2-5', action: exportFile },
		{ text: 'Export game file', id: 'export-game', alias: '2-6', action: exportGameFile },
	]
});
