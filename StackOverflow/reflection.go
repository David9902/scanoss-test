//TAKEN FROM https://stackoverflow.com/questions/73372033/assigning-to-type-definition-using-reflection-in-go
// Get value to assign and its type
vValue := reflect.ValueOf(getInteger())
tValue := vValue.Type()

// Check if the field can be assigned the value; if it can then do so
// Otherwise, check if a conversion can be assigned
vType := vField.Type()
if tValue.AssignableTo(vType) {
	vField.Set(vValue)
} else if vValue.CanConvert(vType) {
	vField.Set(vValue.Convert(vType))
}