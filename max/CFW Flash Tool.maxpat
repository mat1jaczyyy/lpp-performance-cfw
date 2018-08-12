{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 8,
			"minor" : 0,
			"revision" : 0,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"rect" : [ 204.0, 79.0, 914.0, 891.0 ],
		"openrect" : [ 0.0, 0.0, 172.0, 170.0 ],
		"bgcolor" : [ 0.062745, 0.062745, 0.062745, 1.0 ],
		"editing_bgcolor" : [ 0.062745, 0.062745, 0.062745, 1.0 ],
		"bglocked" : 0,
		"openinpresentation" : 1,
		"default_fontsize" : 10.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial Bold",
		"gridonopen" : 1,
		"gridsize" : [ 8.0, 8.0 ],
		"gridsnaponopen" : 1,
		"objectsnaponopen" : 1,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"lefttoolbarpinned" : 0,
		"toptoolbarpinned" : 0,
		"righttoolbarpinned" : 0,
		"bottomtoolbarpinned" : 0,
		"toolbars_unpinned_last_save" : 0,
		"tallnewobj" : 0,
		"boxanimatetime" : 500,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"style" : "",
		"subpatcher_template" : "",
		"title" : "CFW Flash Tool",
		"boxes" : [ 			{
				"box" : 				{
					"id" : "obj-6",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "", "int", "int" ],
					"patching_rect" : [ 224.5, 420.0, 46.0, 20.0 ],
					"presentation_rect" : [ 224.5, 420.0, 46.0, 20.0 ],
					"style" : "",
					"text" : "change"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-120",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "bang", "bang" ],
					"patching_rect" : [ 346.5, 158.658203, 32.0, 20.0 ],
					"presentation_rect" : [ 346.5, 158.658203, 32.0, 20.0 ],
					"style" : "",
					"text" : "t b b"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-119",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 359.5, 207.329102, 56.0, 20.0 ],
					"presentation_rect" : [ 359.5, 207.329102, 56.0, 20.0 ],
					"style" : "",
					"text" : "set Done!"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-118",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 301.5, 266.329102, 83.0, 20.0 ],
					"presentation_rect" : [ 301.5, 266.329102, 83.0, 20.0 ],
					"style" : "",
					"text" : "set Preparing..."
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-116",
					"linecount" : 3,
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 34.5, 107.329102, 59.75, 42.0 ],
					"presentation_linecount" : 3,
					"presentation_rect" : [ 34.5, 107.329102, 59.75, 42.0 ],
					"style" : "",
					"text" : "if $i2 then pause else out2 bang"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-111",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 224.5, 499.329102, 122.0, 20.0 ],
					"presentation_rect" : [ 224.5, 499.329102, 122.0, 20.0 ],
					"style" : "",
					"text" : "prepend set Uploading:"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-110",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 224.5, 472.329102, 116.0, 20.0 ],
					"presentation_rect" : [ 224.5, 472.329102, 116.0, 20.0 ],
					"style" : "",
					"text" : "tosymbol @separator"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-109",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 224.5, 446.658203, 59.0, 20.0 ],
					"presentation_rect" : [ 224.5, 446.658203, 59.0, 20.0 ],
					"style" : "",
					"text" : "append %"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-108",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 34.5, 393.158203, 45.0, 20.0 ],
					"presentation_rect" : [ 34.5, 393.158203, 45.0, 20.0 ],
					"style" : "",
					"text" : "pipe 10"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-107",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "resume", "pause" ],
					"patching_rect" : [ 34.5, 358.329102, 84.0, 20.0 ],
					"presentation_rect" : [ 34.5, 358.329102, 84.0, 20.0 ],
					"style" : "",
					"text" : "t resume pause"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-106",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 12.0, 471.329102, 47.0, 20.0 ],
					"presentation_rect" : [ 12.0, 471.329102, 47.0, 20.0 ],
					"style" : "",
					"text" : "midiout"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-105",
					"linecount" : 2,
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 34.5, 319.158203, 73.0, 31.0 ],
					"presentation_linecount" : 2,
					"presentation_rect" : [ 34.5, 319.158203, 73.0, 31.0 ],
					"style" : "",
					"text" : "if $i1 == 247 then $i1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-104",
					"linecount" : 3,
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 12.0, 201.329102, 34.0, 42.0 ],
					"presentation_linecount" : 3,
					"presentation_rect" : [ 12.0, 201.329102, 34.0, 42.0 ],
					"style" : "",
					"text" : "if $i2 then $i1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-103",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "int", "int" ],
					"patching_rect" : [ 136.25, 313.329102, 29.5, 20.0 ],
					"presentation_rect" : [ 136.25, 313.329102, 29.5, 20.0 ],
					"style" : "",
					"text" : "t i i"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-102",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 146.75, 354.158203, 34.0, 20.0 ],
					"presentation_rect" : [ 146.75, 354.158203, 34.0, 20.0 ],
					"style" : "",
					"text" : "* 100"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-101",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 3,
					"outlettype" : [ "bang", "bang", "int" ],
					"patching_rect" : [ 75.25, 286.329102, 80.0, 20.0 ],
					"presentation_rect" : [ 75.25, 286.329102, 80.0, 20.0 ],
					"style" : "",
					"text" : "uzi 21474836 0"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-100",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "bang", "int" ],
					"patching_rect" : [ 75.25, 251.329102, 31.0, 20.0 ],
					"presentation_rect" : [ 75.25, 251.329102, 31.0, 20.0 ],
					"style" : "",
					"text" : "t b 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-98",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "bang", "pause" ],
					"patching_rect" : [ 75.25, 158.658203, 54.0, 20.0 ],
					"presentation_rect" : [ 75.25, 158.658203, 54.0, 20.0 ],
					"style" : "",
					"text" : "t b pause"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-95",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 210.0, 276.329102, 44.0, 20.0 ],
					"presentation_rect" : [ 210.0, 276.329102, 44.0, 20.0 ],
					"style" : "",
					"text" : "route 0"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-94",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 224.5, 393.158203, 29.5, 20.0 ],
					"presentation_rect" : [ 224.5, 393.158203, 29.5, 20.0 ],
					"style" : "",
					"text" : "/"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-93",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "int", "int" ],
					"patching_rect" : [ 199.5, 236.329102, 29.5, 20.0 ],
					"presentation_rect" : [ 199.5, 236.329102, 29.5, 20.0 ],
					"style" : "",
					"text" : "t i i"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-88",
					"linecount" : 3,
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 75.25, 201.329102, 36.0, 42.0 ],
					"presentation_rect" : [ 75.25, 201.329102, 36.0, 42.0 ],
					"style" : "",
					"text" : "if !$i2 then bang"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-86",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 5,
					"outlettype" : [ "bang", "int", "bang", "stop", "int" ],
					"patching_rect" : [ 138.5, 158.658203, 85.0, 20.0 ],
					"presentation_rect" : [ 138.5, 158.658203, 85.0, 20.0 ],
					"style" : "",
					"text" : "t b 0 b stop 0"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-83",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 3,
					"outlettype" : [ "bang", "bang", "int" ],
					"patching_rect" : [ 138.5, 207.329102, 80.0, 20.0 ],
					"presentation_rect" : [ 138.5, 207.329102, 80.0, 20.0 ],
					"style" : "",
					"text" : "uzi 21474836 0"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-78",
					"maxclass" : "newobj",
					"numinlets" : 3,
					"numoutlets" : 3,
					"outlettype" : [ "int", "bang", "bang" ],
					"patching_rect" : [ 12.0, 74.0, 145.5, 20.0 ],
					"presentation_rect" : [ 12.0, 74.0, 145.5, 20.0 ],
					"style" : "",
					"text" : "filein"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-75",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 50.75, 434.158203, 115.0, 20.0 ],
					"presentation_rect" : [ 50.75, 434.158203, 115.0, 20.0 ],
					"style" : "",
					"text" : "port \"Launchpad Pro\""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-72",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 12.0, 41.0, 81.0, 20.0 ],
					"presentation_rect" : [ 12.0, 41.0, 81.0, 20.0 ],
					"style" : "",
					"text" : "if $i2 then read"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-71",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 427.15387, 358.329102, 466.0, 20.0 ],
					"presentation_rect" : [ 427.15387, 358.329102, 466.0, 20.0 ],
					"style" : "",
					"text" : "script sendbox upload ignoreclick 1, script sendbox upload bgcolor 0.062745 0.062745 0.062745"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-56",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 409.0, 330.829102, 466.0, 20.0 ],
					"presentation_rect" : [ 409.0, 330.829102, 466.0, 20.0 ],
					"style" : "",
					"text" : "script sendbox upload ignoreclick 0, script sendbox upload bgcolor 0.011765 0.396078 0.752941"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-53",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 409.0, 391.329102, 64.0, 20.0 ],
					"presentation_rect" : [ 409.0, 391.329102, 64.0, 20.0 ],
					"save" : [ "#N", "thispatcher", ";", "#Q", "end", ";" ],
					"style" : "",
					"text" : "thispatcher"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-52",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 346.5, 40.329102, 61.0, 20.0 ],
					"presentation_rect" : [ 346.5, 40.329102, 61.0, 20.0 ],
					"style" : "",
					"text" : "delay 3000"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-51",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 449.0, 239.329102, 64.0, 20.0 ],
					"presentation_rect" : [ 449.0, 239.329102, 64.0, 20.0 ],
					"style" : "",
					"text" : "set Ready!"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-50",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 474.0, 266.329102, 154.0, 20.0 ],
					"presentation_rect" : [ 474.0, 266.329102, 154.0, 20.0 ],
					"style" : "",
					"text" : "set \"Waiting for Launchpad...\""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-49",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 449.0, 201.329102, 44.0, 20.0 ],
					"presentation_rect" : [ 449.0, 201.329102, 44.0, 20.0 ],
					"style" : "",
					"text" : "route 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-29",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 449.0, 141.329102, 67.0, 20.0 ],
					"presentation_rect" : [ 449.0, 141.329102, 67.0, 20.0 ],
					"style" : "",
					"text" : "routepass 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-22",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 4,
					"outlettype" : [ "bang", "stop", "bang", "int" ],
					"patching_rect" : [ 419.0, 40.329102, 64.0, 20.0 ],
					"presentation_rect" : [ 419.0, 40.329102, 64.0, 20.0 ],
					"style" : "",
					"text" : "t b stop b 0"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-21",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 449.0, 173.329102, 50.0, 20.0 ],
					"presentation_rect" : [ 449.0, 173.329102, 50.0, 20.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-3",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 449.0, 114.329102, 186.0, 20.0 ],
					"presentation_rect" : [ 449.0, 114.329102, 186.0, 20.0 ],
					"style" : "",
					"text" : "zl compare append \"Launchpad Pro\""
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.062745, 0.062745, 0.062745, 1.0 ],
					"bgoncolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"id" : "obj-87",
					"ignoreclick" : 1,
					"maxclass" : "textbutton",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "", "", "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 12.0, 6.914551, 59.851868, 26.170898 ],
					"presentation" : 1,
					"presentation_rect" : [ 39.0, 83.625, 96.0, 32.0 ],
					"style" : "",
					"text" : "UPLOAD",
					"texton" : "UPLOAD",
					"textoncolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"varname" : "upload"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.062745, 0.062745, 0.062745, 0.0 ],
					"fontface" : 0,
					"fontname" : "Ableton Sans",
					"id" : "obj-33",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 474.0, 299.829102, 118.0, 16.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 4.0, 59.0, 164.0, 16.0 ],
					"style" : "",
					"text" : "Waiting for Launchpad...",
					"textcolor" : [ 0.862745, 0.862745, 0.862745, 1.0 ],
					"textjustification" : 1
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-26",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 449.0, 88.329102, 49.0, 20.0 ],
					"presentation_rect" : [ 449.0, 88.329102, 49.0, 20.0 ],
					"style" : "",
					"text" : "midiinfo"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-24",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 419.0, 10.0, 55.0, 20.0 ],
					"presentation_rect" : [ 419.0, 10.0, 55.0, 20.0 ],
					"style" : "",
					"text" : "loadbang"
				}

			}
, 			{
				"box" : 				{
					"angle" : 270.0,
					"grad1" : [ 0.039216, 0.039216, 0.039216, 1.0 ],
					"grad2" : [ 0.039216, 0.039216, 0.039216, 1.0 ],
					"id" : "obj-32",
					"maxclass" : "panel",
					"mode" : 1,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 689.65387, 107.329102, 147.0, 70.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 0.0, 28.0, 172.0, 124.0 ],
					"proportion" : 0.39,
					"rounded" : 0,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.062745, 0.062745, 0.062745, 0.0 ],
					"fontface" : 0,
					"fontname" : "Ableton Sans",
					"id" : "obj-1",
					"linecount" : 16,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 675.65387, 59.329102, 175.0, 166.0 ],
					"presentation" : 1,
					"presentation_linecount" : 16,
					"presentation_rect" : [ 0.0, 3.0, 172.0, 166.0 ],
					"style" : "",
					"text" : "Custom Firmware Flash Tool\nfor Launchpad Pro\n\n\n\n\n\n\n\n\n\n\n\n\n\nDeveloped by mat1jaczyyy",
					"textcolor" : [ 0.862745, 0.862745, 0.862745, 1.0 ],
					"textjustification" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-101", 0 ],
					"source" : [ "obj-100", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 1 ],
					"midpoints" : [ 96.75, 276.329102, 65.125, 276.329102, 65.125, 188.329102, 36.5, 188.329102 ],
					"order" : 2,
					"source" : [ "obj-100", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-116", 1 ],
					"midpoints" : [ 96.75, 276.0, 133.75, 276.0, 133.75, 102.329102, 84.75, 102.329102 ],
					"order" : 1,
					"source" : [ "obj-100", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-88", 1 ],
					"midpoints" : [ 96.75, 276.329102, 117.75, 276.329102, 117.75, 192.329102, 101.75, 192.329102 ],
					"order" : 0,
					"source" : [ "obj-100", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-103", 0 ],
					"source" : [ "obj-101", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-94", 0 ],
					"midpoints" : [ 156.25, 380.743652, 234.0, 380.743652 ],
					"source" : [ "obj-102", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-102", 0 ],
					"source" : [ "obj-103", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-78", 0 ],
					"midpoints" : [ 145.75, 342.0, 262.625, 342.0, 262.625, 68.0, 21.5, 68.0 ],
					"source" : [ "obj-103", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-105", 0 ],
					"midpoints" : [ 21.5, 310.743652, 44.0, 310.743652 ],
					"order" : 0,
					"source" : [ "obj-104", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-106", 0 ],
					"order" : 1,
					"source" : [ "obj-104", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-107", 0 ],
					"source" : [ "obj-105", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-101", 0 ],
					"midpoints" : [ 109.0, 386.329102, 28.875, 386.329102, 28.875, 281.329102, 84.75, 281.329102 ],
					"source" : [ "obj-107", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-108", 0 ],
					"source" : [ "obj-107", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-101", 0 ],
					"midpoints" : [ 44.0, 419.158203, 28.375, 419.158203, 28.375, 281.329102, 84.75, 281.329102 ],
					"source" : [ "obj-108", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-110", 0 ],
					"source" : [ "obj-109", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-111", 0 ],
					"source" : [ "obj-110", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-33", 0 ],
					"midpoints" : [ 234.0, 524.329102, 395.75, 524.329102, 395.75, 292.829102, 483.5, 292.829102 ],
					"source" : [ "obj-111", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-101", 0 ],
					"midpoints" : [ 44.0, 281.664551, 84.75, 281.664551 ],
					"order" : 1,
					"source" : [ "obj-116", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-120", 0 ],
					"midpoints" : [ 44.0, 153.493652, 356.0, 153.493652 ],
					"order" : 0,
					"source" : [ "obj-116", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-98", 0 ],
					"source" : [ "obj-116", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-33", 0 ],
					"midpoints" : [ 311.0, 292.579102, 483.5, 292.579102 ],
					"source" : [ "obj-118", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-33", 0 ],
					"midpoints" : [ 369.0, 234.0, 395.0, 234.0, 395.0, 293.0, 483.5, 293.0 ],
					"source" : [ "obj-119", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-119", 0 ],
					"source" : [ "obj-120", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-52", 0 ],
					"midpoints" : [ 356.0, 182.658203, 322.0, 182.658203, 322.0, 29.329102, 356.0, 29.329102 ],
					"source" : [ "obj-120", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-49", 0 ],
					"order" : 0,
					"source" : [ "obj-21", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-72", 1 ],
					"midpoints" : [ 458.5, 197.329102, 315.0, 197.329102, 315.0, 33.0, 83.5, 33.0 ],
					"order" : 1,
					"source" : [ "obj-21", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-21", 0 ],
					"midpoints" : [ 473.5, 81.329102, 435.5, 81.329102, 435.5, 166.329102, 458.5, 166.329102 ],
					"source" : [ "obj-22", 3 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-26", 0 ],
					"source" : [ "obj-22", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-52", 0 ],
					"midpoints" : [ 443.5, 76.329102, 327.5, 76.329102, 327.5, 29.329102, 356.0, 29.329102 ],
					"source" : [ "obj-22", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-52", 0 ],
					"midpoints" : [ 428.5, 72.329102, 334.25, 72.329102, 334.25, 29.329102, 356.0, 29.329102 ],
					"source" : [ "obj-22", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-22", 0 ],
					"source" : [ "obj-24", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-3", 0 ],
					"source" : [ "obj-26", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-21", 0 ],
					"source" : [ "obj-29", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-29", 0 ],
					"source" : [ "obj-3", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-50", 0 ],
					"order" : 0,
					"source" : [ "obj-49", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-51", 0 ],
					"order" : 0,
					"source" : [ "obj-49", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-56", 0 ],
					"midpoints" : [ 458.5, 228.579102, 418.5, 228.579102 ],
					"order" : 1,
					"source" : [ "obj-49", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-71", 0 ],
					"midpoints" : [ 483.5, 233.329102, 436.65387, 233.329102 ],
					"order" : 1,
					"source" : [ "obj-49", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-33", 0 ],
					"source" : [ "obj-50", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-33", 0 ],
					"midpoints" : [ 458.5, 292.079102, 483.5, 292.079102 ],
					"source" : [ "obj-51", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-22", 0 ],
					"midpoints" : [ 356.0, 66.329102, 415.25, 66.329102, 415.25, 34.329102, 428.5, 34.329102 ],
					"source" : [ "obj-52", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-53", 0 ],
					"source" : [ "obj-56", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-109", 0 ],
					"source" : [ "obj-6", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-53", 0 ],
					"midpoints" : [ 436.65387, 384.329102, 418.5, 384.329102 ],
					"source" : [ "obj-71", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-78", 0 ],
					"source" : [ "obj-72", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-106", 0 ],
					"midpoints" : [ 60.25, 462.243652, 21.5, 462.243652 ],
					"source" : [ "obj-75", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 0 ],
					"source" : [ "obj-78", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-116", 0 ],
					"midpoints" : [ 84.75, 98.664551, 44.0, 98.664551 ],
					"source" : [ "obj-78", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-86", 0 ],
					"source" : [ "obj-78", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-93", 0 ],
					"source" : [ "obj-83", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-104", 1 ],
					"midpoints" : [ 164.5, 188.493652, 36.5, 188.493652 ],
					"order" : 2,
					"source" : [ "obj-86", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-116", 1 ],
					"midpoints" : [ 164.5, 188.329102, 133.875, 188.329102, 133.875, 102.329102, 84.75, 102.329102 ],
					"order" : 1,
					"source" : [ "obj-86", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-118", 0 ],
					"midpoints" : [ 181.0, 198.993652, 311.0, 198.993652 ],
					"order" : 0,
					"source" : [ "obj-86", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-21", 0 ],
					"midpoints" : [ 214.0, 187.658203, 437.25, 187.658203, 437.25, 166.329102, 458.5, 166.329102 ],
					"source" : [ "obj-86", 4 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-52", 0 ],
					"midpoints" : [ 197.5, 192.329102, 282.25, 192.329102, 282.25, 23.329102, 356.0, 23.329102 ],
					"source" : [ "obj-86", 3 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-75", 0 ],
					"midpoints" : [ 181.0, 198.329102, 289.0, 198.329102, 289.0, 360.329102, 209.0, 360.329102, 209.0, 425.329102, 60.25, 425.329102 ],
					"order" : 1,
					"source" : [ "obj-86", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-83", 0 ],
					"source" : [ "obj-86", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-88", 1 ],
					"midpoints" : [ 164.5, 192.493652, 101.75, 192.493652 ],
					"order" : 0,
					"source" : [ "obj-86", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-72", 0 ],
					"source" : [ "obj-87", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-100", 0 ],
					"source" : [ "obj-88", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-78", 0 ],
					"midpoints" : [ 209.0, 265.329102, 262.75, 265.329102, 262.75, 68.0, 21.5, 68.0 ],
					"source" : [ "obj-93", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-95", 0 ],
					"source" : [ "obj-93", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-6", 0 ],
					"source" : [ "obj-94", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-94", 1 ],
					"source" : [ "obj-95", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-83", 0 ],
					"midpoints" : [ 119.75, 199.493652, 148.0, 199.493652 ],
					"source" : [ "obj-98", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-88", 0 ],
					"source" : [ "obj-98", 0 ]
				}

			}
 ],
		"dependency_cache" : [  ],
		"autosave" : 0
	}

}
