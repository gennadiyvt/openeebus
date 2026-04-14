namespace mu_mpc_test {
namespace send {

static constexpr char electrical_connection_parameter_description_reply[] = R"({
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
              "feature": 1
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
              "feature": 4
            }
          ]
        },
        {
          "msgCounter": 9
        },
        {
          "msgCounterReference": 7
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
                "electricalConnectionParameterDescriptionListData": [
                  {
                    "electricalConnectionParameterDescriptionData": [
                      [
                        {
                          "electricalConnectionId": 0
                        },
                        {
                          "parameterId": 0
                        },
                        {
                          "measurementId": 0
                        },
                        {
                          "voltageType": "ac"
                        },
                        {
                          "acMeasuredPhases": "a"
                        },
                        {
                          "acMeasuredInReferenceTo": "neutral"
                        },
                        {
                          "acMeasurementType": "real"
                        },
                        {
                          "acMeasurementVariant": "rms"
                        }
                      ],
                      [
                        {
                          "electricalConnectionId": 0
                        },
                        {
                          "parameterId": 1
                        },
                        {
                          "measurementId": 1
                        },
                        {
                          "voltageType": "ac"
                        },
                        {
                          "acMeasuredPhases": "a"
                        },
                        {
                          "acMeasuredInReferenceTo": "neutral"
                        },
                        {
                          "acMeasurementType": "real"
                        },
                        {
                          "acMeasurementVariant": "rms"
                        }
                      ],
                      [
                        {
                          "electricalConnectionId": 0
                        },
                        {
                          "parameterId": 2
                        },
                        {
                          "measurementId": 2
                        },
                        {
                          "voltageType": "ac"
                        },
                        {
                          "acMeasurementType": "real"
                        }
                      ],
                      [
                        {
                          "electricalConnectionId": 0
                        },
                        {
                          "parameterId": 3
                        },
                        {
                          "measurementId": 3
                        },
                        {
                          "voltageType": "ac"
                        },
                        {
                          "acMeasurementType": "real"
                        }
                      ],
                      [
                        {
                          "electricalConnectionId": 0
                        },
                        {
                          "parameterId": 4
                        },
                        {
                          "measurementId": 4
                        },
                        {
                          "voltageType": "ac"
                        },
                        {
                          "acMeasuredPhases": "a"
                        },
                        {
                          "acMeasurementType": "real"
                        },
                        {
                          "acMeasurementVariant": "rms"
                        }
                      ],
                      [
                        {
                          "electricalConnectionId": 0
                        },
                        {
                          "parameterId": 5
                        },
                        {
                          "measurementId": 5
                        },
                        {
                          "voltageType": "ac"
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
