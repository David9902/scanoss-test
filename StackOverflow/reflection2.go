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

type Foo struct {
	Code  Provider
	Value string
}

foo := &Foo{
	Code:  Provider(0),
	Value: "derp",
}

var scnr sql.Scanner
scannerType := reflect.TypeOf(&scnr).Elem()

tType := reflect.TypeOf(foo)
tField := tType.Field(0)
fmt.Printf("Field %s, of type %s, kind %s\n",
	tField.Name, tField.Type, tField.Type.Kind())

	import (
		"math"
	)
	
	func main() {
		LevenshteinDistance("stackoverflow", "stackexchange")
	}
	
	func LevenshteinDistance(s string, t string) int {
		if s == t {
			return 0
		}
		if len(s) == 0 {
			return len(t)
		}
		if len(t) == 0 {
			return len(s)
		}
	
		v0 := make([]int, len(t)+1)
		v1 := make([]int, len(t)+1)
	
		for i := 0; i < len(v0); i++ {
			v0[i] = i
		}
	
		for i := 0; i < len(s); i++ {
			v1[0] = i + 1
			for j := 0; j < len(t); j++ {
				cost := 0
				if s[i] != t[j] {
					cost = 1
				}
				v1[j+1] = int(math.Min(float64(v1[j]+1), math.Min(float64(v0[j+1]+1), float64(v0[j]+cost))))
			}
	
			for j := 0; j < len(v0); j++ {
				v0[j] = v1[j]
			}
		}
		return v1[len(t)]
	}