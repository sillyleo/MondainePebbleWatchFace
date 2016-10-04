//z weather simple
module.exports = 
[
	{//info
		"type": "heading", 
		"defaultValue": "Zszen Swiss Square Modify" 
	}, 
	{//info
		"type": "text", 
		"defaultValue": "config v 1.0" 
	},
	{
		"type": "section",
		"items": [
			{
				"type": "select",
				"messageKey": "CONFIG_watchfaceType",
				"defaultValue": "0",
				"label": "Watchface Type",
				"options": [
					{ 
						"label": "White & Logo",
						"value": 0 
					},
					{ 
						"label": "White",
						"value": 1 
					},
					{ 
						"label": "Black & Logo",
						"value": 2 
					},
					{ 
						"label": "Black",
						"value": 3 
					}
				]
			},
			{
				"type": "select",
				"messageKey": "CONFIG_secondType",
				"defaultValue": "0",
				"label": "Second Type",
				"options": [
					{ 
						"label": "Second",
						"value": 0 
					},
					{ 
						"label": "5 Seconds",
						"value": 1 
					},
					{ 
						"label": "Hidden",
						"value": 2
					}
				]
			},
// 			{
// 				"type": "color",
// 				"messageKey": "CONFIG_hourColor",
// 				"defaultValue": "0x000000",
// 				"label": "Hour Color",
// 				"sunlight": false,
// 				"allowGray": true
// 			},
// 			{
// 				"type": "color",
// 				"messageKey": "CONFIG_minuteColor",
// 				"defaultValue": "0x000000",
// 				"label": "Minute Color",
// 				"sunlight": false,
// 				"allowGray": true
// 			},
// 			{
// 				"type": "color",
// 				"messageKey": "CONFIG_secondColor",
// 				"defaultValue": "0xff0000",
// 				"label": "Second Color",
// 				"sunlight": false,
// 				"allowGray": true
// 			},
			{//submit button
				"type": "submit",
				"defaultValue": "Save & Restart"
			}
		]
	}
];
