package main

import (
	"flag"
	"log"
	"path/filepath"
	"strconv"
	"strings"
	"io/ioutil"
	"text/template"
	"os"

	"github.com/extrame/xls"
)

const GLOBAL_SHEET_MIN_ROW = 1
const GLOBAL_SHEET_MIN_COL = 2

type sheet_name struct {
	Zh_name string
	En_name string
}

type Field struct {
	Type     string
	Name     string
	Min, Max string
}

type sheet_config struct {
	FirstRow       int
	FileName       string //output FileName.cpp
	OutPutFilePath string
	Sheets         map[string]string
}

type sub_ouput_template struct {
	sheet_name
	Fields []Field
	Key    Field
}

type output_template struct {
	Class_Name  string
	OutPath     string
	Sub_Configs []sub_ouput_template
}

var tmpl_flag = flag.String("tc", "./default.tmpl", "template file path")
var config_flag = flag.String("c", "/root/*", "xls_file match_arg. EG:/root/*.xls")
var target_dir = flag.String("d", "./", "output file dir")
var tmpl *template.Template
func init() {
	flag.Parse()
	data, err := ioutil.ReadFile(*tmpl_flag)
	if err != nil {
		panic(err)
	}
	funcs := make(template.FuncMap)
	funcs["tolower"] = strings.ToLower
	funcs["toupper"] = strings.ToUpper
	tmpl, err = template.New("test").Funcs(funcs).Parse(string(data))
	if err != nil {
		panic(err)
	}
}


func parse_file(config *output_template) {
	h_file, err := os.Create("./" + strings.ToLower(config.Class_Name) + ".h")
	if err != nil {
		log.Println(err)
		return
	}
	defer h_file.Close()
	
	cpp_file, err := os.Create("./" + strings.ToLower(config.Class_Name) + ".cpp")
	if err != nil {
		log.Println(err)
		return
	}
	defer cpp_file.Close()
	
	err = tmpl.ExecuteTemplate(h_file, "H", config)
	if err != nil {
		log.Println(err)
		return
	}
	
	err = tmpl.ExecuteTemplate(cpp_file, "CPP", config)
	if err != nil {
		log.Println(err)
		return
	}
}

func match(cs, title, name, data string) (*Field, bool) {
	if !strings.Contains(cs, "s") {
		return nil, false
	}

	ret_field := &Field{Name: name}
	switch name {
	case "hp":
		fallthrough
	case "attack":
		fallthrough
	case "fangyu":
		fallthrough
	case "mingzhong":
		fallthrough
	case "shanbi":
		fallthrough
	case "baoji":
		fallthrough
	case "jianren":
		ret_field.Type = "Int64"
	default:
		_, err := strconv.ParseInt(data, 10, 32)
		if err != nil {
			if strings.Contains(strings.ToLower(cs), "item") {
				ret_field.Type = "ItemConfigData"
			}else{
				ret_field.Type = "std::string"
			}
		} else {
			ret_field.Type = "Int32"
		}
	}
	return ret_field, true
}

func build_sheet(xlsx_file *xls.WorkBook, config *sheet_config) *output_template {

	out_config := &output_template{Class_Name: config.FileName, Sub_Configs: make([]sub_ouput_template, 0)}

	for i := 1; i <= xlsx_file.NumSheets(); i++ {
		sheet := xlsx_file.GetSheet(i)
		if sheet == nil {
			continue
		}

		if sheet.MaxRow < uint16(config.FirstRow+2) {
			continue
		}

		first_row := sheet.Row(config.FirstRow - 1)
		second_row := sheet.Row(config.FirstRow)
		third_row := sheet.Row(config.FirstRow + 1)
		first_data_row := sheet.Row(config.FirstRow + 2)
		if first_row == nil || second_row == nil || third_row == nil || first_data_row == nil {
			continue
		}

		sub_sheet := &sub_ouput_template{}
		class_name, ok := config.Sheets[sheet.Name]
		if !ok {
			continue
		}
		
		sub_sheet.En_name = class_name
		sub_sheet.Zh_name = sheet.Name
			
		for j := first_row.FirstCol(); j <= first_row.LastCol(); j++ {
			f, ok := match(first_row.Col(j), second_row.Col(j), third_row.Col(j), first_data_row.Col(j))
			if ok {
				if j == first_row.FirstCol() {
					sub_sheet.Key = *f
				}
				sub_sheet.Fields = append(sub_sheet.Fields, *f)
			}
		}

		out_config.Sub_Configs = append(out_config.Sub_Configs, *sub_sheet)
	}
	out_config.Class_Name = config.FileName
	out_config.OutPath = config.OutPutFilePath
	return out_config
}

func build_xlsx(file_name string) {
	f, err := xls.Open(file_name, "utf-8")
	if err != nil {
		log.Println(err)
		return
	}

	if f.NumSheets() < 2 {
		log.Println(file_name + " sheet_count < 2")
		return
	}

	global_sheet := f.GetSheet(0)
	if global_sheet == nil {
		log.Println("global_sheet is nil")
	}

	if global_sheet.MaxRow < GLOBAL_SHEET_MIN_ROW {
		log.Println(file_name + " global_sheet MaxRow < GLOBAL_SHEET_MIN_ROW")
		log.Println(global_sheet.MaxRow)
		return
	}

	first_row := global_sheet.Row(0)
	second_row := global_sheet.Row(1)
	if first_row == nil || second_row == nil {
		log.Println("first or second global_config_rolw is nil")
		return
	}

	ret_sheet_config := &sheet_config{Sheets: make(map[string]string)}

	for i := first_row.FirstCol(); i <= first_row.LastCol(); i++ {
		Zh_name := first_row.Col(i)
		En_name := second_row.Col(i)

		if Zh_name == "" || En_name == "" {
			log.Println("continue Zh_name:" + Zh_name + "/" + "En_name:" + En_name)
			continue
		}

		switch i {
		case 0:
			ret_sheet_config.FileName = En_name
		case 1:
			ret_sheet_config.OutPutFilePath = En_name
		case 2:
			ret_sheet_config.FirstRow, err = strconv.Atoi(En_name)
			if err != nil {
				log.Println(err)
				return
			}
		default:
			ret_sheet_config.Sheets[Zh_name] = En_name
		}
	}
	parse_file(build_sheet(f, ret_sheet_config))
	return
}

func main() {

	file_names, err := filepath.Glob(*config_flag)
	if err != nil {
		panic(err)
	}

	funcs := make(template.FuncMap)
	funcs["tolower"] = strings.ToLower
	funcs["toupper"] = strings.ToUpper

	for _, v := range file_names {
		build_xlsx(v)
		log.Println(v)
	}
}
