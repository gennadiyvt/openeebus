namespace mu_mpc_test {
namespace send {

static constexpr char measurement_description_reply[] = R"({
  "datagram": [
    {
      "header": [
        {
          "specificationVersion": "1.3.0"
        },
        {
          "addressSource": [
            {
              "device": "d:_n:HeatPump_123456789"
            },
            {
              "entity": [
                1
              ]
            },
            {
              "feature": 2
            }
          ]
        },
        {
          "addressDestination": [
            {
              "device": "d:_n:HEMS_123456789"
            },
            {
              "entity": [
                1
              ]
            },
            {
              "feature": 6
            }
          ]
        },
        {
          "msgCounter": 11
        },
        {
          "msgCounterReference": 9
        },
        {
          "cmdClassifier": "reply"
        }
      ]
    },
    {
      "payload": [
        {
          "cmd": [
            [
              {
                "measurementDescriptionListData": [
                  {
                    "measurementDescriptionData": [
                      [
                        {
                          "measurementId": 0
                        },
                        {
                          "measurementType": "power"
                        },
                        {
                          "commodityType": "electricity"
                        },
                        {
                          "unit": "W"
                        },
                        {
                          "scopeType": "acPowerTotal"
                        }
                      ],
                      [
                        {
                          "measurementId": 1
                        },
                        {
                          "measurementType": "power"
                        },
                        {
                          "commodityType": "electricity"
                        },
                        {
                          "unit": "W"
                        },
                        {
                          "scopeType": "acPower"
                        }
                      ],
                      [
                        {
                          "measurementId": 2
                        },
                        {
                          "measurementType": "energy"
                        },
                        {
                          "commodityType": "electricity"
                        },
                        {
                          "unit": "Wh"
                        },
                        {
                          "scopeType": "acEnergyConsumed"
                        }
                      ],
                      [
                        {
                          "measurementId": 3
                        },
                        {
                          "measurementType": "energy"
                        },
                        {
                          "commodityType": "electricity"
                        },
                        {
                          "unit": "Wh"
                        },
                        {
                          "scopeType": "acEnergyProduced"
                        }
                      ],
                      [
                        {
                          "measurementId": 4
                        },
                        {
                          "measurementType": "current"
                        },
                        {
                          "commodityType": "electricity"
                        },
                        {
                          "unit": "A"
                        },
                        {
                          "scopeType": "acCurrent"
                        }
                      ],
                      [
                        {
                          "measurementId": 5
                        },
                        {
                          "measurementType": "frequency"
                        },
                        {
                          "commodityType": "electricity"
                        },
                        {
                          "unit": "Hz"
                        },
                        {
                          "scopeType": "acFrequency"
                        }
                      ]
                    ]
                  }
                ]
              }
            ]
          ]
        }
      ]
    }
  ]
})";

}  // namespace send
}  // namespace mu_mpc_test
