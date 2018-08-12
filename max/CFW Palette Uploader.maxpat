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
		"rect" : [ 34.0, 79.0, 1298.0, 963.0 ],
		"openrect" : [ 0.0, 0.0, 172.0, 169.0 ],
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
		"boxes" : [ 			{
				"box" : 				{
					"border" : 2.0,
					"id" : "obj-3",
					"linecolor" : [ 0.062745, 0.062745, 0.062745, 1.0 ],
					"maxclass" : "live.line",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 404.65387, 123.0, 93.0, 5.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 5.0, 82.0, 162.0, 5.0 ]
				}

			}
, 			{
				"box" : 				{
					"border" : 2.0,
					"id" : "obj-49",
					"linecolor" : [ 0.062745, 0.062745, 0.062745, 1.0 ],
					"maxclass" : "live.line",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 389.65387, 108.0, 93.0, 5.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 5.0, 106.0, 162.0, 5.0 ]
				}

			}
, 			{
				"box" : 				{
					"activebgcolor" : [ 0.74902, 0.74902, 0.74902, 0.0 ],
					"bordercolor" : [ 1.0, 1.0, 1.0, 0.0 ],
					"focusbordercolor" : [ 0.196078, 0.196078, 0.196078, 0.0 ],
					"fontname" : "Ableton Sans",
					"fontsize" : 10.0,
					"id" : "obj-85",
					"maxclass" : "live.numbox",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 382.65387, 132.0, 32.0, 13.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 108.888435, 88.0, 20.0, 13.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_type" : 1,
							"parameter_unitstyle" : 0,
							"parameter_mmin" : 1.0,
							"parameter_longname" : "live.numbox",
							"parameter_mmax" : 3.0,
							"parameter_shortname" : "live.numbox"
						}

					}
,
					"textcolor" : [ 0.2, 0.5, 0.8, 1.0 ],
					"textjustification" : 2,
					"varname" : "channel"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.062745, 0.062745, 0.062745, 0.0 ],
					"fontface" : 0,
					"fontname" : "Ableton Sans",
					"id" : "obj-1",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 426.6539, 132.0, 72.0, 16.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 47.536564, 87.0, 71.0, 16.0 ],
					"style" : "",
					"text" : "Palette Index",
					"textcolor" : [ 0.862745, 0.862745, 0.862745, 1.0 ]
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
					"patching_rect" : [ 630.25, 11.585448, 64.0, 16.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 7.0, 36.0, 65.0, 16.0 ],
					"style" : "",
					"text" : "MIDI Output",
					"textcolor" : [ 0.862745, 0.862745, 0.862745, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-28",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 607.5, 248.0, 72.0, 20.0 ],
					"presentation_rect" : [ 607.5, 248.0, 72.0, 20.0 ],
					"style" : "",
					"text" : "prepend port"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-27",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 585.25, 174.625, 109.0, 20.0 ],
					"presentation_rect" : [ 585.25, 174.625, 109.0, 20.0 ],
					"style" : "",
					"text" : "set Launchpad Open"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-26",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 612.0, 93.0, 49.0, 20.0 ],
					"presentation_rect" : [ 612.0, 93.0, 49.0, 20.0 ],
					"style" : "",
					"text" : "midiinfo"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-25",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "bang", "bang", "bang" ],
					"patching_rect" : [ 558.5, 65.0, 72.5, 20.0 ],
					"presentation_rect" : [ 558.5, 65.0, 72.5, 20.0 ],
					"style" : "",
					"text" : "t b b b"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-24",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 558.5, 39.670898, 55.0, 20.0 ],
					"presentation_rect" : [ 558.5, 39.670898, 55.0, 20.0 ],
					"style" : "",
					"text" : "loadbang"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.011765, 0.396078, 0.752941, 1.0 ],
					"bgoncolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"id" : "obj-4",
					"maxclass" : "textbutton",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "", "", "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 558.5, 6.5, 61.5, 26.170897 ],
					"presentation" : 1,
					"presentation_rect" : [ 92.0, 32.914551, 70.0, 20.170898 ],
					"style" : "",
					"text" : "RESCAN",
					"texton" : "Rescan",
					"textoncolor" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-31",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 597.75, 133.625, 189.0, 20.0 ],
					"presentation_rect" : [ 597.75, 133.625, 189.0, 20.0 ],
					"style" : "",
					"text" : "set Launchpad Open Standalone Port"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.2, 0.2, 0.2, 1.0 ],
					"bgfillcolor_angle" : 270.0,
					"bgfillcolor_autogradient" : 0,
					"bgfillcolor_color" : [ 0.2, 0.2, 0.2, 1.0 ],
					"bgfillcolor_color1" : [ 0.32549, 0.345098, 0.372549, 1.0 ],
					"bgfillcolor_color2" : [ 0.290196, 0.309804, 0.301961, 1.0 ],
					"bgfillcolor_proportion" : 0.39,
					"bgfillcolor_type" : "color",
					"id" : "obj-21",
					"items" : "<empty>",
					"maxclass" : "umenu",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "int", "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 558.5, 219.0, 117.0, 20.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 10.0, 58.0, 152.0, 20.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-20",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 10.184692, 548.0, 48.0, 20.0 ],
					"presentation_rect" : [ 10.184692, 548.0, 48.0, 20.0 ],
					"style" : "",
					"text" : "midiout"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-19",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "bang", "" ],
					"patching_rect" : [ 80.653854, 257.625, 29.5, 20.0 ],
					"presentation_rect" : [ 80.653854, 257.625, 29.5, 20.0 ],
					"style" : "",
					"text" : "t b l"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-18",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 80.653854, 228.0, 233.0, 20.0 ],
					"presentation_rect" : [ 80.653854, 228.0, 233.0, 20.0 ],
					"style" : "",
					"text" : "if $i1 == 127 then 240 82 69 84 73 78 65 125 247"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-17",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 351.15387, 228.0, 170.0, 20.0 ],
					"presentation_rect" : [ 351.15387, 228.0, 170.0, 20.0 ],
					"style" : "",
					"text" : "if $i1 % 64 == 63 then append 247"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-16",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 382.65387, 155.625, 29.5, 20.0 ],
					"presentation_rect" : [ 382.65387, 155.625, 29.5, 20.0 ],
					"style" : "",
					"text" : "- 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-13",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 143.653885, 184.0, 258.0, 20.0 ],
					"presentation_rect" : [ 143.653885, 184.0, 258.0, 20.0 ],
					"style" : "",
					"text" : "if $i1 % 64 == 0 then 240 82 69 84 73 78 65 61 $i2"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-12",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 10.184692, 228.0, 45.0, 20.0 ],
					"presentation_rect" : [ 10.184692, 228.0, 45.0, 20.0 ],
					"style" : "",
					"text" : "zl iter 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-11",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 54.653854, 43.0, 151.0, 20.0 ],
					"presentation_rect" : [ 54.653854, 43.0, 151.0, 20.0 ],
					"style" : "",
					"text" : "240 82 69 84 73 78 65 123 247"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-9",
					"linecount" : 14,
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 10.184692, 301.0, 540.0, 165.0 ],
					"presentation_linecount" : 14,
					"presentation_rect" : [ 10.184692, 301.0, 540.0, 165.0 ],
					"style" : "",
					"text" : "240 82 69 84 73 78 65 123 247 240 82 69 84 73 78 65 61 2 0 0 0 0 1 7 7 7 2 31 31 31 3 63 63 63 4 63 18 18 5 63 0 0 6 21 0 0 7 6 0 0 8 63 46 26 9 63 20 0 10 21 7 0 11 9 6 0 12 63 63 18 13 63 63 0 14 21 21 0 15 6 6 0 16 33 63 18 17 20 63 0 18 7 21 0 19 4 10 0 20 18 63 18 21 0 63 0 22 0 21 0 23 0 6 0 24 18 63 23 25 0 63 6 26 0 21 3 27 0 6 0 28 18 63 33 29 0 63 21 30 0 21 7 31 0 7 4 32 18 63 45 33 0 63 37 34 0 21 13 35 0 6 4 36 18 48 63 37 0 41 63 38 0 16 20 39 0 3 6 40 18 33 63 41 0 21 63 42 0 7 21 43 0 1 6 44 18 18 63 45 0 0 63 46 0 0 21 47 0 0 6 48 33 18 63 49 20 0 63 50 6 0 24 51 3 0 11 52 63 18 63 53 63 0 63 54 21 0 21 55 6 0 6 56 63 18 33 57 63 0 20 58 21 0 7 59 8 0 4 60 63 5 0 61 37 13 0 62 29 20 0 63 16 24 0 247 240 82 69 84 73 78 65 61 2 64 0 14 0 65 0 21 13 66 0 20 31 67 0 0 63 68 0 17 19 69 9 0 50 70 31 31 31 71 7 7 7 72 63 0 0 73 46 63 11 74 43 58 1 75 24 63 2 76 3 34 0 77 0 63 33 78 0 41 63 79 0 10 63 80 15 0 63 81 30 0 63 82 43 6 30 83 15 8 0 84 63 18 0 85 33 55 1 86 28 63 5 87 0 63 0 88 14 63 9 89 21 63 27 90 13 63 50 91 22 34 63 92 12 20 48 93 33 31 57 94 52 7 63 95 63 0 22 96 63 31 0 97 45 43 0 98 35 63 0 99 32 22 1 100 14 10 0 101 4 18 3 102 3 19 13 103 5 5 10 104 5 7 22 105 25 14 6 106 41 0 2 107 54 20 15 108 53 26 6 109 63 55 9 110 39 55 11 111 25 44 3 112 7 7 11 113 54 63 26 114 31 63 46 115 38 37 63 116 35 25 63 117 15 15 15 118 28 28 28 119 55 63 63 120 39 0 0 121 13 0 0 122 6 51 0 123 1 16 0 124 45 43 0 125 15 12 0 126 44 23 0 127 18 5 0 247 240 82 69 84 73 78 65 125 247"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-7",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 5,
					"outlettype" : [ "int", "int", "int", "int", "int" ],
					"patching_rect" : [ 80.653854, 155.625, 82.000031, 20.0 ],
					"presentation_rect" : [ 80.653854, 155.625, 82.000031, 20.0 ],
					"style" : "",
					"text" : "t i i i i i"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-6",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 3,
					"outlettype" : [ "bang", "bang", "int" ],
					"patching_rect" : [ 54.653854, 101.625, 45.0, 20.0 ],
					"presentation_rect" : [ 54.653854, 101.625, 45.0, 20.0 ],
					"style" : "",
					"text" : "uzi 128"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.011765, 0.396078, 0.752941, 1.0 ],
					"bgoncolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"id" : "obj-87",
					"maxclass" : "textbutton",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "", "", "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 10.184692, 6.5, 59.851868, 26.170898 ],
					"presentation" : 1,
					"presentation_rect" : [ 39.0, 111.625, 96.0, 32.0 ],
					"style" : "",
					"text" : "UPLOAD",
					"texton" : "UPLOAD",
					"textoncolor" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-93",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 80.653854, 130.295898, 29.5, 20.0 ],
					"presentation_rect" : [ 80.653854, 130.295898, 29.5, 20.0 ],
					"style" : "",
					"text" : "- 1"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.231373, 0.121569, 0.305882, 1.0 ],
					"color" : [ 0.466667, 0.254902, 0.607843, 1.0 ],
					"id" : "obj-89",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 4,
					"outlettype" : [ "", "", "", "" ],
					"patching_rect" : [ 10.184692, 74.625, 85.703743, 20.0 ],
					"presentation_rect" : [ 10.184692, 74.625, 85.703743, 20.0 ],
					"saved_object_attributes" : 					{
						"embed" : 0
					}
,
					"style" : "",
					"text" : "coll palette"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.466667, 0.254902, 0.607843, 1.0 ],
					"bgcolor2" : [ 0.231373, 0.121569, 0.305882, 1.0 ],
					"bgfillcolor_angle" : 270.0,
					"bgfillcolor_autogradient" : 0.0,
					"bgfillcolor_color" : [ 0.290196, 0.309804, 0.301961, 1.0 ],
					"bgfillcolor_color1" : [ 0.466667, 0.254902, 0.607843, 1.0 ],
					"bgfillcolor_color2" : [ 0.231373, 0.121569, 0.305882, 1.0 ],
					"bgfillcolor_proportion" : 0.39,
					"bgfillcolor_type" : "gradient",
					"gradient" : 1,
					"id" : "obj-91",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 10.184692, 43.0, 35.0, 20.0 ],
					"presentation_rect" : [ 10.184692, 43.0, 35.0, 20.0 ],
					"style" : "",
					"text" : "read"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-10",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 10.184692, 257.625, 61.0, 20.0 ],
					"presentation_rect" : [ 10.184692, 257.625, 61.0, 20.0 ],
					"style" : "",
					"text" : "append $1"
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
					"patching_rect" : [ 201.0, 132.0, 128.0, 128.0 ],
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
					"id" : "obj-30",
					"linecount" : 16,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 170.653885, 93.0, 175.0, 166.0 ],
					"presentation" : 1,
					"presentation_linecount" : 16,
					"presentation_rect" : [ 0.0, 3.0, 172.0, 166.0 ],
					"style" : "",
					"text" : "Retina Palette Uploader for Launchpad Pro Custom Firmware\n\n\n\n\n\n\n\n\n\n\n\n\n\nDeveloped by mat1jaczyyy",
					"textcolor" : [ 0.862745, 0.862745, 0.862745, 1.0 ],
					"textjustification" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-9", 0 ],
					"source" : [ "obj-10", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-9", 1 ],
					"midpoints" : [ 64.153854, 68.0, 540.684692, 68.0 ],
					"source" : [ "obj-11", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-10", 0 ],
					"source" : [ "obj-12", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-12", 0 ],
					"midpoints" : [ 153.153885, 210.0, 19.684692, 210.0 ],
					"source" : [ "obj-13", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-13", 1 ],
					"source" : [ "obj-16", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-9", 0 ],
					"midpoints" : [ 360.65387, 291.0, 19.684692, 291.0 ],
					"source" : [ "obj-17", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-19", 0 ],
					"source" : [ "obj-18", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-12", 0 ],
					"midpoints" : [ 100.653854, 284.0, 526.669281, 284.0, 526.669281, 210.0, 19.684692, 210.0 ],
					"source" : [ "obj-19", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-9", 0 ],
					"midpoints" : [ 90.153854, 291.8125, 19.684692, 291.8125 ],
					"source" : [ "obj-19", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-28", 0 ],
					"source" : [ "obj-21", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-25", 0 ],
					"source" : [ "obj-24", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-21", 0 ],
					"source" : [ "obj-25", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-26", 0 ],
					"source" : [ "obj-25", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-27", 0 ],
					"order" : 1,
					"source" : [ "obj-25", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 0 ],
					"midpoints" : [ 594.75, 89.8125, 607.25, 89.8125 ],
					"order" : 0,
					"source" : [ "obj-25", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-21", 0 ],
					"midpoints" : [ 621.5, 121.5, 568.0, 121.5 ],
					"source" : [ "obj-26", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-21", 0 ],
					"midpoints" : [ 594.75, 205.3125, 568.0, 205.3125 ],
					"source" : [ "obj-27", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-20", 0 ],
					"midpoints" : [ 617.0, 542.0, 19.684692, 542.0 ],
					"source" : [ "obj-28", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-21", 0 ],
					"midpoints" : [ 607.25, 161.670898, 568.0, 161.670898 ],
					"source" : [ "obj-31", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-24", 0 ],
					"source" : [ "obj-4", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-93", 0 ],
					"source" : [ "obj-6", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-10", 0 ],
					"midpoints" : [ 137.403877, 189.0, 109.0, 189.0, 109.0, 189.0, 70.0, 189.0, 70.0, 252.0, 19.684692, 252.0 ],
					"source" : [ "obj-7", 3 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-13", 0 ],
					"source" : [ "obj-7", 4 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"midpoints" : [ 105.903862, 220.3125, 360.65387, 220.3125 ],
					"source" : [ "obj-7", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-18", 0 ],
					"source" : [ "obj-7", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-89", 0 ],
					"midpoints" : [ 121.653869, 185.295898, 2.184753, 185.295898, 2.184753, 69.295898, 19.684692, 69.295898 ],
					"source" : [ "obj-7", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"source" : [ "obj-85", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-11", 0 ],
					"midpoints" : [ 19.684692, 38.335449, 64.153854, 38.335449 ],
					"order" : 0,
					"source" : [ "obj-87", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-91", 0 ],
					"order" : 1,
					"source" : [ "obj-87", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-12", 0 ],
					"source" : [ "obj-89", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-6", 0 ],
					"source" : [ "obj-89", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-20", 0 ],
					"source" : [ "obj-9", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-89", 0 ],
					"source" : [ "obj-91", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-7", 0 ],
					"source" : [ "obj-93", 0 ]
				}

			}
 ],
		"parameters" : 		{
			"obj-85" : [ "live.numbox", "live.numbox", 0 ],
			"parameterbanks" : 			{

			}

		}
,
		"dependency_cache" : [ 			{
				"name" : "NovationPalette",
				"bootpath" : "~/Plugins",
				"patcherrelativepath" : "../../../Plugins",
				"type" : "TEXT",
				"implicit" : 1
			}
 ],
		"autosave" : 0
	}

}
