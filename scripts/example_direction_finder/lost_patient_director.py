#!/usr/bin/env python2
from __future__ import print_function
from temboo.Library.Google.Directions import GetWalkingDirections
from temboo.core.session import TembooSession

# Create a session with your Temboo account details
session = TembooSession("willprice", "myFirstApp", "f4696cb40b254f89814e61745c885369")

# Instantiate the Choreo
getWalkingDirectionsChoreo = GetWalkingDirections(session)

# Get an InputSet object for the Choreo
getWalkingDirectionsInputs = getWalkingDirectionsChoreo.new_input_set()

# Set the Choreo inputs
getWalkingDirectionsInputs.set_Origin("40.7180,-74.001")
getWalkingDirectionsInputs.set_Destination("40.7160,-74.0037")

# Execute the Choreo
getWalkingDirectionsResults = getWalkingDirectionsChoreo.execute_with_results(getWalkingDirectionsInputs)

# Print the Choreo outputs
print("Response: " + getWalkingDirectionsResults.get_Response())
