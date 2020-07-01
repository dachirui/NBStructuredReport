#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */
#include "dcmtk/ofstd/ofconsol.h"     /* for COUT */

#include "dcmtk/dcmsr/dsrdoc.h"       /* for main interface class DSRDocument */
#include "dcmtk/dcmsr/codes/ucum.h"

#include "dcmtk/dcmdata/dctk.h"       /* for typical set of "dcmdata" headers */

#include <nlohmann/json.hpp>
#include <string>




using namespace std;
using json = nlohmann::json;

int main()
{
	std::ifstream i("/folder/eform.json");
	
	json j;
	i >> j;

	std::string string_sex = j["patient_data"]["sex"]["value"];
	std::string string_patient_id = j["patient_data"]["patient_id"]["value"];
	
	if (string_sex == "Male"){
			string_sex = "M";
		}
	else if(string_sex == "Female"){
			string_sex = "F";
		}
	else {
			string_sex = "O";	
		}
	
	const char *patient_id = string_patient_id.c_str();
	const char *sex = string_sex.c_str();
	
	DSRDocument *doc = new DSRDocument();

	doc->createNewDocument(DSRTypes::DT_BasicTextSR);
	doc->setPatientName("_");
	doc->setPatientSex(sex);
	doc->setPatientID(patient_id);

	doc->getTree().addContentItem(DSRTypes::RT_isRoot, DSRTypes::VT_Container);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("1", "PrimageNB", "Neuroblastoma structured report"));
	
	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("439401001", "SRT", "Diagnosis"));
	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("MTHU021540", "LN", "Associated Sympthoms"));

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_DateTime, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("432213005", "SRT", "Diagnosis date"));

	bool checker = j["diagnosis"]["associated_symptoms"]["diagnosis_date"]["value"].is_null();
	if (checker == false) {

		std::string string_diagnosis_date = j["diagnosis"]["associated_symptoms"]["diagnosis_date"]["value"];
		std::string chars = "-T:.Z";
		for (char c : chars) {
			string_diagnosis_date.erase(std::remove(string_diagnosis_date.begin(), 
				string_diagnosis_date.end(), c), string_diagnosis_date.end());
		}
		std::string sub_string_diagnosis_date = string_diagnosis_date.substr(0, 12);
		const char *diagnosis_date = sub_string_diagnosis_date.c_str();
		doc->getTree().getCurrentContentItem().setObservationDateTime(diagnosis_date);
		doc->getTree().getCurrentContentItem().setStringValue(diagnosis_date);
	}


	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_DateTime, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("Symptom_onset", "COVIDCRFRAPID", "First symptoms date"));
	checker = j["diagnosis"]["associated_symptoms"]["diagnosis_date"]["value"].is_null();
	if (checker == false) {
		std::string string_first_symptoms_date = j["diagnosis"]["associated_symptoms"]["first_symptoms_date"]["value"];
		std::string chars = "-T:.Z";
		for (char c : chars) {
			string_first_symptoms_date.erase(std::remove(string_first_symptoms_date.begin(), string_first_symptoms_date.end(), c), string_first_symptoms_date.end());
		}
		std::string sub_string_first_symptoms_date = string_first_symptoms_date.substr(0, 12);
		const char *first_symptoms_date = sub_string_first_symptoms_date.c_str();
		doc->getTree().getCurrentContentItem().setObservationDateTime(first_symptoms_date);
		doc->getTree().getCurrentContentItem().setStringValue(first_symptoms_date);





		/*const char *first_symptoms_date = string_first_symptoms_date.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(first_symptoms_date);*/
	}
	
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C48619", "NCIt", "Incidental finding"));
	bool bool_yes = j["diagnosis"]["associated_symptoms"]["incidental_finding"]["yes"].get<bool>();
	bool bool_no = j["diagnosis"]["associated_symptoms"]["incidental_finding"]["no"].get<bool>();
	bool bool_unknown = j["diagnosis"]["associated_symptoms"]["incidental_finding"]["unknown"].get<bool>();
	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}


	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("423493009", "SRT", "Age at diagnosis"));

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("Years", "ADW", "Years"));
	checker = j["diagnosis"]["associated_symptoms"]["age_at_diagnosis"]["years"]["value"].is_null();
	if (checker == false) {
		int number_years = j["diagnosis"]["associated_symptoms"]["age_at_diagnosis"]["years"]["value"].get<int>();
		std::string string_years = to_string(number_years);
		const char *years = string_years.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(years);
	}


	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("LA9001-4", "LN", "Months"));
	checker = j["diagnosis"]["associated_symptoms"]["age_at_diagnosis"]["months"]["value"].is_null();
	if (checker == false) {
		int number_months = j["diagnosis"]["associated_symptoms"]["age_at_diagnosis"]["months"]["value"].get<int>();
		std::string string_months = to_string(number_months);
		const char *months = string_months.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(months);
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("LA9001-4", "LN", "Total months"));
	checker = j["diagnosis"]["associated_symptoms"]["age_at_diagnosis"]["total_months"]["value"].is_null();
	if (checker == false) {
		int number_total_months_date = j["diagnosis"]["associated_symptoms"]["age_at_diagnosis"]["total_months"]["value"].get<int>();
		std::string string_total_months_date = to_string(number_total_months_date);
		const char *total_months_date = string_total_months_date.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(total_months_date);
	}

	//##############    LTS SECTION        ###########################
	doc->getTree().goUp();

	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RisqueVital", "ONTOPARON", "LTS (Life Threatening Symptoms)"));

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("2", "PrimageNB", "Intraspinal neuroblastoma"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["lts"]["intraspinal_neuroblastoma"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["lts"]["intraspinal_neuroblastoma"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["lts"]["intraspinal_neuroblastoma"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("726582005", "SRT", "Opiate tratment"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["lts"]["opiate_treatment"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["lts"]["opiate_treatment"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["lts"]["opiate_treatment"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("419907002", "SRT", "Vomiting needing nasogastric/IV support"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["lts"]["vomiting"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["lts"]["vomiting"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["lts"]["vomiting"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("WTLOS", "AIR", "Weight loss > 10% body weight"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["lts"]["weight_loss"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["lts"]["weight_loss"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["lts"]["weight_loss"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("271825005", "SRT", "Respiratory distress"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["lts"]["respiratory_distress"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["lts"]["respiratory_distress"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["lts"]["respiratory_distress"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID39097", "RADLEX", "Hypertension"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["lts"]["hypertension"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["lts"]["hypertension"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["lts"]["hypertension"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("3", "PrimageNB", "IVC (Inferior vena cava) compression +- leg edema"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["lts"]["ivc_compresion"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["lts"]["ivc_compresion"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["lts"]["ivc_compresion"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("4", "PrimageNB", "Creatinine > 2x Upper Limit Normal"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["lts"]["creatinine"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["lts"]["creatinine"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["lts"]["creatinine"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("5", "PrimageNB", "Urine output < 2 mL/Kg/day"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["lts"]["urine_output"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["lts"]["urine_output"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["lts"]["urine_output"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("69758005", "SRT", "Hydroureter / hydronephrosis"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["lts"]["hidroureter"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["lts"]["hidroureter"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["lts"]["hidroureter"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}
	
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("6", "PrimageNB", "Abnormal liver function > 2x Upper Limit Normal"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["lts"]["abnormal_liver_function"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["lts"]["abnormal_liver_function"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["lts"]["abnormal_liver_function"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("67406007", "SRT", "Evidence of DIC (Disseminated Intravascular Coagulation)"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["lts"]["evidence_of_dic"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["lts"]["evidence_of_dic"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["lts"]["evidence_of_dic"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("7", "PrimageNB", "Platelets < 50 x 109/L"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["lts"]["platelets"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["lts"]["platelets"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["lts"]["platelets"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("8", "PrimageNB", "Bladder/bowel dysfunction secondary to mass effect"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["lts"]["bladder_bowel_dysfunction"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["lts"]["bladder_bowel_dysfunction"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["lts"]["bladder_bowel_dysfunction"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("9", "PrimageNB", "Very large tumor volume causing concern of possible tumor rupture and/or the possible rapid development of systemic upset"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["lts"]["large_tumor_volume"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["lts"]["large_tumor_volume"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["lts"]["large_tumor_volume"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("272419002", "SRT", "Large tumor volume"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["lts"]["large_tumor_volume"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["lts"]["large_tumor_volume"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["lts"]["large_tumor_volume"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("394617004", "SRT", "Result"));
	std::string string_symptoms_res = j["diagnosis"]["associated_symptoms"]["lts"]["result"]["value"];
	const char *symptoms_res = string_symptoms_res.c_str();
	doc->getTree().getCurrentContentItem().setStringValue(symptoms_res);

	//##############    END LTS SECTION        ###########################

	//##############     OTHER ASSOCIATED SYMPTOMS SECTION        ###########################
	doc->getTree().goUp();

	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("LP133892-2", "LN", "Other associated symptoms"));

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("10077133", "MEDR", "Spinal cord syndrome"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["other"]["spinal_cord_syndrome"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["other"]["spinal_cord_syndrome"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["other"]["spinal_cord_syndrome"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("230350000", "SRT", "OMS(Opsoclonus - Myocionus Syndrome)"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["other"]["oms"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["other"]["oms"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["other"]["oms"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("81669005", "SRT", "NF(Neurofibromatosis)"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["other"]["neurofibromatosis"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["other"]["neurofibromatosis"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["other"]["neurofibromatosis"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}


	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("71286001", "SRT", "Symptomatic spinal cord compression"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["other"]["spinal_cord_compression"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["other"]["spinal_cord_compression"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["other"]["spinal_cord_compression"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("10055422", "MEDR", "Watery diarrhea"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["other"]["watery_diarrhoea"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["other"]["watery_diarrhoea"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["other"]["watery_diarrhoea"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("271730003", "SRT", "Horner's syndrome"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["other"]["horners_syndrome"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["other"]["horners_syndrome"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["other"]["horners_syndrome"]["unknown"].get<bool>();
	if (bool_yes == true) {

		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("Xa4hE", "RCD", "Genetic syndrome"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["other"]["genetic_syndrome"]["result"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["other"]["genetic_syndrome"]["result"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["other"]["genetic_syndrome"]["result"]["unknown"].get<bool>();
	if (bool_yes == true) {
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("394617004", "SRT", "Result"));
		std::string string_value_gs = j["diagnosis"]["associated_symptoms"]["other"]["genetic_syndrome"]["value"]["value"];
		const char *value_gs = string_value_gs.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(value_gs);
		doc->getTree().goUp();

	}
	else if (bool_no == true) {
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("394617004", "SRT", "Result"));
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
		doc->getTree().goUp();
	}
	else if (bool_unknown == true) {
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("394617004", "SRT", "Result"));
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
		doc->getTree().goUp();
	}
	
	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("74964007", "SRT", "Others"));
	bool_yes = j["diagnosis"]["associated_symptoms"]["other"]["others"]["result"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["associated_symptoms"]["other"]["others"]["result"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["associated_symptoms"]["other"]["others"]["result"]["unknown"].get<bool>();
	if (bool_yes == true) {
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("394617004", "SRT", "Result"));
		std::string string_value_gs = j["diagnosis"]["associated_symptoms"]["other"]["others"]["value"]["value"];
		const char *value_gs = string_value_gs.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(value_gs);
		doc->getTree().goUp();

	}
	else if (bool_no == true) {
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("394617004", "SRT", "Result"));
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
		doc->getTree().goUp();
	}
	else if (bool_unknown == true) {
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("394617004", "SRT", "Result"));
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
		doc->getTree().goUp();
	}


	doc->getTree().goUp();
	
//##############     END OTHER ASSOCIATED SYMPTOMS SECTION        ###########################//


doc->getTree().goUp();

//############################               LABORATORY #############################
doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("261904005", "SRT", "Laboratory"));
	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("10024048", "MEDdra", "LDH"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
		bool_yes = j["diagnosis"]["laboratory"]["ldh"]["evaluated"]["yes"].get<bool>();
		bool_no = j["diagnosis"]["laboratory"]["ldh"]["evaluated"]["no"].get<bool>();
		if (bool_yes == true) {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("767525000", "SRT", "LDH unit"));
			std::string ldh_unit = j["diagnosis"]["laboratory"]["ldh"]["unit"]["value"];
			if (ldh_unit == "IU/L") {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("767525000", "SRT", "IU/L"));
			}
			else if (ldh_unit == "μmol/sl") {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("10", "PrimageNB", "μmol/sl"));
			}


			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C25706", "NCIt", "Upper normal value"));
			checker = j["diagnosis"]["laboratory"]["ldh"]["upper_normal_value"]["value"].is_null();
			if (checker == false) {
				int num_upper_normal_value = j["diagnosis"]["laboratory"]["ldh"]["upper_normal_value"]["value"].get<int>();
				std::string string_upper_normal_value = to_string(num_upper_normal_value);
				const char *upper_normal_value = string_upper_normal_value.c_str();
				doc->getTree().getCurrentContentItem().setStringValue(upper_normal_value);
			}
			

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("250644007", "SRT", "LDH Result"));
			checker = j["diagnosis"]["laboratory"]["ldh"]["ldh_result"]["value"].is_null();
			if (checker == false) {
				int num_ldh_result = j["diagnosis"]["laboratory"]["ldh"]["ldh_result"]["value"].get<int>();
				std::string string_ldh_result = to_string(num_ldh_result);
				const char *ldh_result = string_ldh_result.c_str();
				doc->getTree().getCurrentContentItem().setStringValue(ldh_result);
			}

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("396697000", "SRT", "Elevated"));
			bool_yes = j["diagnosis"]["laboratory"]["ldh"]["elevated"]["yes"].get<bool>();
			bool_no = j["diagnosis"]["laboratory"]["ldh"]["elevated"]["no"].get<bool>();
			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
				doc->getTree().goUp();
			}
			else if(bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
				doc->getTree().goUp();
			}
		}
		else if (bool_no == true) {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
		}

		//TABLA DOCS
		doc->getTree().goUp();
		doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C19097", "NCIt", "Pathology tumor"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
		bool_yes = j["diagnosis"]["laboratory"]["pathology_tumor"]["evaluated"]["yes"].get<bool>();
		bool_no = j["diagnosis"]["laboratory"]["pathology_tumor"]["evaluated"]["no"].get<bool>();
		if (bool_yes == true) {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
					   			 
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_DateTime, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("11", "PrimageNB", "Biopsy date"));
			bool checker = j["diagnosis"]["laboratory"]["pathology_tumor"]["biopsy_date"]["value"].is_null();
			if (checker == false) {

				std::string string_biopsy_date = j["diagnosis"]["laboratory"]["pathology_tumor"]["biopsy_date"]["value"];
				std::string chars = "-T:.Z";
				for (char c : chars) {
					string_biopsy_date.erase(std::remove(string_biopsy_date.begin(), string_biopsy_date.end(), c), string_biopsy_date.end());
				}
				std::string sub_string_biopsy_date = string_biopsy_date.substr(0, 12);
				const char *biopsy_date = sub_string_biopsy_date.c_str();
				doc->getTree().getCurrentContentItem().setObservationDateTime(biopsy_date);
				doc->getTree().getCurrentContentItem().setStringValue(biopsy_date);
			}
					   			 		  
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("MTHU015568", "LN", "Histology type"));
			std::string string_histology_type = j["diagnosis"]["laboratory"]["pathology_tumor"]["histology_type"]["value"];
			const char *histology_type = string_histology_type.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(histology_type);

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("263522009", "SRT", "Degree of differentiation"));
			std::string string_grade_of_differentiation = j["diagnosis"]["laboratory"]["pathology_tumor"]["grade_of_differentiation"]["value"];
			const char *grade_of_differentiation = string_grade_of_differentiation.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(grade_of_differentiation);

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C106317", "NCIt", "Mitosis-Karyorrhexis Index (MKI)"));
			std::string string_mki = j["diagnosis"]["laboratory"]["pathology_tumor"]["mki"]["value"];
			const char *mki = string_mki.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(mki);

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("4I2..00", "RCTV2", "Type of sample"));
			std::string string_type_of_sample = j["diagnosis"]["laboratory"]["pathology_tumor"]["type_of_sample"]["value"];
			const char *type_of_sample = string_type_of_sample.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(type_of_sample);

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("12", "PrimageNB", "Biopsy type"));
			std::string string_biopsy_type = j["diagnosis"]["laboratory"]["pathology_tumor"]["biopsy_type"]["value"];
			const char *biopsy_type = string_biopsy_type.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(biopsy_type);
			doc->getTree().goUp();
		}
		
		doc->getTree().goUp();
		doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("13", "PrimageNB", "Bone marrow studies"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
		bool_yes = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["evaluated"]["yes"].get<bool>();
		bool_no = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["evaluated"]["no"].get<bool>();
		if (bool_yes == true) {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_DateTime, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("410672004", "SRT", "Bone marrow studies date"));
			checker = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["date"]["value"].is_null();
			if (checker == false) {
				std::string string_date = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["date"]["value"];				
				std::string chars = "-T:.Z";
				for (char c : chars) {
					string_date.erase(std::remove(string_date.begin(), string_date.end(), c), string_date.end());
				}
				std::string sub_string_date = string_date.substr(0, 12);
				const char *date = sub_string_date.c_str();
				doc->getTree().getCurrentContentItem().setObservationDateTime(date);
				doc->getTree().getCurrentContentItem().setStringValue(date);
			}
			

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("10005982", "MEDdra", " Bone marrow aspiration procedure"));
			bool_yes = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["aspirate"]["evaluated"]["yes"].get<bool>();
			bool_no = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["aspirate"]["evaluated"]["no"].get<bool>();
			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));

				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("394617004", "SRT", "Result"));
				bool_yes = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["aspirate"]["result"]["yes"].get<bool>();
				bool_no = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["aspirate"]["result"]["no"].get<bool>();
				bool_unknown = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["aspirate"]["result"]["inconclusive"].get<bool>();
				if (bool_yes == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("10828004", "SRT", "Positive"));
				}
				else if (bool_no == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "Negative"));
				}
				else {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("419984006", "SRT", "Inconclusive"));
				}

			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));

			}

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);		
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("50590007", "SRT", "Trephine biopsy"));
			bool_yes = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["trephine"]["evaluated"]["yes"].get<bool>();
			bool_no = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["trephine"]["evaluated"]["no"].get<bool>();
			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));

				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("394617004", "SRT", "Result"));
				bool_yes = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["trephine"]["result"]["yes"].get<bool>();
				bool_no = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["trephine"]["result"]["no"].get<bool>();
				bool_unknown = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["trephine"]["result"]["inconclusive"].get<bool>();
				if (bool_yes == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Positive"));
				}
				else if (bool_no == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "Negative"));
				}
				else {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("419984006", "SRT", "Inconclusive"));
				}

			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));

			}

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("10079987", "MEDdra", "Minimal Residual Disease"));
			bool_yes = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["minimal_residual_disease"]["evaluated"]["yes"].get<bool>();
			bool_no = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["minimal_residual_disease"]["evaluated"]["no"].get<bool>();
			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));

				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("260686004", "SRT", "Method"));
				std::string string_method = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["minimal_residual_disease"]["method"]["value"];
				if (string_method == "qRT-PCR") {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("201606016242659673", "IOBC", "qRT-PCR"));

					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("14", "PrimageNB", "TH above SIOPEN cut point"));
					std::string th_value = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["minimal_residual_disease"]["result_qrt_pcr"]["th"]["value"];
					if (th_value == "Yes") {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

					}
					else if (th_value == "No") {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

					}

					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("15", "PrimageNB", "PHOX2B above SIOPEN cut point"));
					std::string phox2b_value = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["minimal_residual_disease"]["result_qrt_pcr"]["phox2b"]["value"];
					if (phox2b_value == "Yes") {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

					}
					else if (phox2b_value == "No") {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
					}

					
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("16", "PrimageNB", "Immunocytology result"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LOINC", "No result"));
					
				}
				else if (string_method == "Immunocytology") {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("17", "PrimageNB", "Immunocytology"));
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("14", "PrimageNB", "TH above SIOPEN cut point"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LOINC", "No result"));


					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("15", "PrimageNB", "PHOX2B above SIOPEN cut point"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LOINC", "No result"));



					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("16", "PrimageNB", "Immunocytology result"));
					std::string inmuno_value = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["minimal_residual_disease"]["result_immunocitology"]["value"];
					if (inmuno_value == "Positive") {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("10828004", "SRT", "Positive"));

					}
					else if (inmuno_value == "Negative") {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("260385009", "SRT", "Negative"));
					}
				}
				else if (string_method == "qRT-PCR and Immunocytology") {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("18", "PrimageNB", "qRT-PCR and Immunocytology"));
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("14", "PrimageNB", "TH above SIOPEN cut point"));
					std::string th_value = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["minimal_residual_disease"]["result_qrt_pcr"]["th"]["value"];
					if (th_value == "Yes") {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

					}
					else if (th_value == "No") {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

					}

					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("15", "PrimageNB", "PHOX2B above SIOPEN cut point"));
					std::string phox2b_value = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["minimal_residual_disease"]["result_qrt_pcr"]["phox2b"]["value"];
					if (phox2b_value == "Yes") {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

					}
					else if (phox2b_value == "No") {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
					}

					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("16", "PrimageNB", "Immunocytology result"));
					std::string inmuno_value = j["diagnosis"]["laboratory"]["pathology_bone_marrow"]["minimal_residual_disease"]["result_immunocitology"]["value"];
					if (inmuno_value == "Positive") {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("10828004", "SRT", "Positive"));

					}
					else if (inmuno_value == "Negative") {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("260385009", "SRT", "Negative"));
					}
				}		
			}
		}
		else if (bool_no == true) {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
			
		}
		
		doc->getTree().goUp();

		doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C135727", "NCIt", "Liquid biopsy (Peripheral blood)"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
		bool_yes = j["diagnosis"]["laboratory"]["liquid_biopsy"]["evaluated"]["yes"].get<bool>();
		bool_no = j["diagnosis"]["laboratory"]["liquid_biopsy"]["evaluated"]["no"].get<bool>();
		if (bool_yes == true) {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
			doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("10079987", "MEDdra", "Minimal Residual Disease"));
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("10079987", "MEDdra", "Minimal Residual Disease"));
			bool_yes = j["diagnosis"]["laboratory"]["liquid_biopsy"]["minimal_residual_disease"]["evaluated"]["yes"].get<bool>();
			bool_no = j["diagnosis"]["laboratory"]["liquid_biopsy"]["minimal_residual_disease"]["evaluated"]["no"].get<bool>();
			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));

				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("260686004", "SRT", "Method"));
				std::string string_method = j["diagnosis"]["laboratory"]["liquid_biopsy"]["minimal_residual_disease"]["method"]["value"];
				if (string_method == "qRT-PCR") {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("201606016242659673", "IOBC", "qRT-PCR"));

					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("14", "PrimageNB", "TH above SIOPEN cut point"));
					std::string th_value = j["diagnosis"]["laboratory"]["liquid_biopsy"]["minimal_residual_disease"]["result_qrt_pcr"]["th"]["value"];
					if (th_value == "Yes") {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

					}
					else if (th_value == "No") {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

					}

					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("15", "PrimageNB", "PHOX2B above SIOPEN cut point"));
					std::string phox2b_value = j["diagnosis"]["laboratory"]["liquid_biopsy"]["minimal_residual_disease"]["result_qrt_pcr"]["phox2b"]["value"];
					if (phox2b_value == "Yes") {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

					}
					else if (phox2b_value == "No") {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
					}	
				}	
				
			}
		}
		else if (bool_no == true) {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));

		}
		doc->getTree().goUp();
		doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("MTHU012766", "LN", "Molecular studies"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
		bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["evaluated"]["yes"].get<bool>();
		bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["evaluated"]["no"].get<bool>();
		if (bool_yes == true) {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("4I2..00", "RCTV2", "Sample type"));
			std::string molecular_sample_type = j["diagnosis"]["laboratory"]["molecular_studies"]["sample_type"]["value"];
			if (molecular_sample_type == "Primary tumor") {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("42697", "OCHV", "Primary tumor"));
			}
			else if (molecular_sample_type == "Bone marrow") {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID38594", "RADLEX", "Bone marrow"));
			}
			else if (molecular_sample_type == "Lymph nodes") {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("D008198", "MESH", "Lymph nodes"));
			}
			else if (molecular_sample_type == "Other metastasis") {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("19", "PrimageNB", "Other metastasis"));
			}

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("23", "PrimageNB", "Status MYCN"));
			bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["status_mycn"]["evaluated"]["yes"].get<bool>();
			bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["status_mycn"]["evaluated"]["no"].get<bool>();
			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));

				bool fish = j["diagnosis"]["laboratory"]["molecular_studies"]["status_mycn"]["method"]["fish"]["value"].get<bool>();
				bool snpa = j["diagnosis"]["laboratory"]["molecular_studies"]["status_mycn"]["method"]["snpa"]["value"].get<bool>();
				bool pcr = j["diagnosis"]["laboratory"]["molecular_studies"]["status_mycn"]["method"]["pcr"]["value"].get<bool>();
				bool southern_blot = j["diagnosis"]["laboratory"]["molecular_studies"]["status_mycn"]["method"]["southern_blot"]["value"].get<bool>();
				bool mlpa = j["diagnosis"]["laboratory"]["molecular_studies"]["status_mycn"]["method"]["mlpa"]["value"].get<bool>();
				bool acgh = j["diagnosis"]["laboratory"]["molecular_studies"]["status_mycn"]["method"]["acgh"]["value"].get<bool>();
				bool icwgs = j["diagnosis"]["laboratory"]["molecular_studies"]["status_mycn"]["method"]["icwgs"]["value"].get<bool>();
				bool deepwgs = j["diagnosis"]["laboratory"]["molecular_studies"]["status_mycn"]["method"]["deepwgs"]["value"].get<bool>();

				if (fish == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("426329006", "SRT", "Method FISH"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (snpa == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("_DDB_G0285111", "PR", "Method SNPa"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (pcr == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("LA26418-6", "LN", "Method PCR"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (southern_blot == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("MI_0103", "MI", "Method Southern Blot"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (mlpa == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("200906058748194996", "IOBC", "Method MLPA"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (acgh == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("ERO_0001818", "ERO", "Method aCGH"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (icwgs == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("21", "PrimageNB", "Method IcWGS"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (deepwgs == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("22", "PrimageNB", "Method deepWGS"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}

				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("23", "PrimageNB", "Status MYCN"));
				std::string str_mycn_status = j["diagnosis"]["laboratory"]["molecular_studies"]["status_mycn"]["status"]["value"];
				const char *mycn_status = str_mycn_status.c_str();
				doc->getTree().getCurrentContentItem().setStringValue(mycn_status);


			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));

			}
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C131136", "NCIt", "Status ALK amplification"));
			bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["amplification"]["evaluated"]["yes"].get<bool>();
			bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["amplification"]["evaluated"]["no"].get<bool>();
			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));

				bool fish = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["amplification"]["method"]["fish"]["value"].get<bool>();
				bool snpa = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["amplification"]["method"]["snpa"]["value"].get<bool>();
				bool pcr = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["amplification"]["method"]["pcr"]["value"].get<bool>();
				bool southern_blot = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["amplification"]["method"]["southern_blot"]["value"].get<bool>();
				bool mlpa = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["amplification"]["method"]["mlpa"]["value"].get<bool>();
				bool acgh = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["amplification"]["method"]["acgh"]["value"].get<bool>();
				bool icwgs = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["amplification"]["method"]["icwgs"]["value"].get<bool>();
				bool deepwgs = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["amplification"]["method"]["deepwgs"]["value"].get<bool>();

				if (fish == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("426329006", "SRT", "Method FISH"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (snpa == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("_DDB_G0285111", "PR", "Method SNPa"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (pcr == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("LA26418-6", "LN", "Method PCR"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (southern_blot == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("MI_0103", "MI", "Method Southern Blot"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (mlpa == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("200906058748194996", "IOBC", "Method MLPA"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (acgh == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("ERO_0001818", "ERO", "Method aCGH"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (icwgs == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("21", "PrimageNB", "Method IcWGS"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (deepwgs == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("22", "PrimageNB", "Method deepWGS"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}

				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C131136", "NCIt", "Status ALK amplification"));
				std::string str_mycn_status = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["amplification"]["status"]["value"];
				const char *mycn_status = str_mycn_status.c_str();
				doc->getTree().getCurrentContentItem().setStringValue(mycn_status);
			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));

			}

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("10071999", "MEDdra", "Status ALK mutation"));
			bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["mutation"]["evaluated"]["yes"].get<bool>();
			bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["mutation"]["evaluated"]["no"].get<bool>();
			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				bool sanger_seq = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["mutation"]["method"]["sanger_seq"]["value"].get<bool>();
				bool ngs = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["mutation"]["method"]["ngs"]["value"].get<bool>();
				bool wes = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["mutation"]["method"]["wes"]["value"].get<bool>();
				bool wgs = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["mutation"]["method"]["wgs"]["value"].get<bool>();
				bool ddpcr = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["mutation"]["method"]["ddpcr"]["value"].get<bool>();

				if (sanger_seq == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("426329006", "SRT", "Method Sanger seq"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (ngs == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("_DDB_G0285111", "PR", "Method NGS panel sequencing"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (wes == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("LA26418-6", "LN", "Method WES"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (wgs == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("MI_0103", "MI", "Method WGS"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (ddpcr == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("200906058748194996", "IOBC", "Method ddpcr"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}

				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("24", "PrimageNB", "Status ALK mutation F114L"));
				bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["mutation"]["f1174"]["yes"].get<bool>();
				bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["mutation"]["f1174"]["no"].get<bool>();
				bool bool_no_result = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["mutation"]["f1174"]["no_result"].get<bool>();

				if (bool_yes == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

				}
				else if (bool_no == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

				}
				else if (bool_no_result == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LN", "No result"));

				}

				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("25", "PrimageNB", "Status ALK mutation F1245Q"));
				bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["mutation"]["f1245"]["yes"].get<bool>();
				bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["mutation"]["f1245"]["no"].get<bool>();
				bool_no_result = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["mutation"]["f1245"]["no_result"].get<bool>();

				if (bool_yes == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

				}
				else if (bool_no == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

				}
				else if (bool_no_result == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LN", "No result"));

				}

				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("26", "PrimageNB", "Status ALK mutation R1275Q"));
				bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["mutation"]["r1275"]["yes"].get<bool>();
				bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["mutation"]["r1275"]["no"].get<bool>();
				bool_no_result = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["mutation"]["r1275"]["no_result"].get<bool>();

				if (bool_yes == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

				}
				else if (bool_no == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

				}
				else if (bool_no_result == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LN", "No result"));

				}


				checker = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["mutation"]["other"]["value"].is_null();
				if (checker == false) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("74964007","SRT", "Other"));
					std::string str_mutation_other = j["diagnosis"]["laboratory"]["molecular_studies"]["status_alk"]["mutation"]["other"]["value"];
					const char *mutation_other = str_mutation_other.c_str();
					doc->getTree().getCurrentContentItem().setStringValue(mutation_other);


				}



			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));

			}

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("88183001", "SRT", "Ploidy"));
			bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["ploidy"]["evaluated"]["yes"].get<bool>();
			bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["ploidy"]["evaluated"]["no"].get<bool>();
			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));

				bool karyotyping = j["diagnosis"]["laboratory"]["molecular_studies"]["ploidy"]["method"]["karyotyping"]["value"].get<bool>();
				bool facs = j["diagnosis"]["laboratory"]["molecular_studies"]["ploidy"]["method"]["facs"]["value"].get<bool>();
				bool snpa = j["diagnosis"]["laboratory"]["molecular_studies"]["ploidy"]["method"]["snpa"]["value"].get<bool>();
				bool acgh = j["diagnosis"]["laboratory"]["molecular_studies"]["ploidy"]["method"]["acgh"]["value"].get<bool>();
				bool icwgs = j["diagnosis"]["laboratory"]["molecular_studies"]["ploidy"]["method"]["icwgs"]["value"].get<bool>();
				bool deepwgs = j["diagnosis"]["laboratory"]["molecular_studies"]["ploidy"]["method"]["deepwgs"]["value"].get<bool>();

				if (karyotyping == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("10023306", "MEDdra", "Method Karyotyping"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (facs == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("200906032506360760", "IOBC", "Method FACS"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (snpa == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("_DDB_G0285111", "PR", "Method SNPa"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (acgh == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("ERO_0001818", "ERO", "Method aCGH"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (icwgs == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("21", "PrimageNB", "Method IcWGS"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (deepwgs == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("22", "PrimageNB", "Method deepWGS"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}

				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("88183001", "SRT", "Ploidy Status"));
				std::string str_ploidy_status = j["diagnosis"]["laboratory"]["molecular_studies"]["ploidy"]["status"]["value"];
				const char *ploidy_status = str_ploidy_status.c_str();
				doc->getTree().getCurrentContentItem().setStringValue(ploidy_status);
			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));

			}

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("200906021998510604", "IOBC", "NCA"));
			bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["nca"]["evaluated"]["yes"].get<bool>();
			bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["nca"]["evaluated"]["no"].get<bool>();
			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));

				bool fish = j["diagnosis"]["laboratory"]["molecular_studies"]["nca"]["method"]["fish"]["value"].get<bool>();
				bool snpa = j["diagnosis"]["laboratory"]["molecular_studies"]["nca"]["method"]["snpa"]["value"].get<bool>();
				bool acgh = j["diagnosis"]["laboratory"]["molecular_studies"]["nca"]["method"]["acgh"]["value"].get<bool>();
				bool icwgs = j["diagnosis"]["laboratory"]["molecular_studies"]["nca"]["method"]["icwgs"]["value"].get<bool>();
				bool deepwgs = j["diagnosis"]["laboratory"]["molecular_studies"]["nca"]["method"]["deepwgs"]["value"].get<bool>();

				if (fish == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("426329006", "SRT", "Method FISH"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (snpa == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("_DDB_G0285111", "PR", "Method SNPa"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (acgh == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("ERO_0001818", "ERO", "Method aCGH"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (icwgs == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("21", "PrimageNB", "Method IcWGS"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (deepwgs == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("22", "PrimageNB", "Method deepWGS"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}

				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("200906021998510604", "IOBC", "NCA Status"));
				bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["nca"]["status"]["yes"].get<bool>();
				bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["nca"]["status"]["no"].get<bool>();
				bool bool_no_result = j["diagnosis"]["laboratory"]["molecular_studies"]["nca"]["status"]["no_result"].get<bool>();

				if (bool_yes == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

				}
				else if (bool_no == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

				}
				else if (bool_no_result == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LN", "No result"));

				}

				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("27", "PrimageNB", "NCA Number of alterations"));
				int num_alterations = j["diagnosis"]["laboratory"]["molecular_studies"]["nca"]["number"]["value"].get<int>();
				std::string string_alterations = to_string(num_alterations);
				const char *alterations = string_alterations.c_str();
				doc->getTree().getCurrentContentItem().setStringValue(alterations);

				for (int i = 0; i < num_alterations; i++) {
					doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C28344", "NCIt", "NCA Alteration"));
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("91272006", "SRT", "Chromosome"));

					std::string str_chr = j["diagnosis"]["laboratory"]["molecular_studies"]["nca"]["alterations"][i]["chromosome"]["value"];
					const char *chr = str_chr.c_str();
					doc->getTree().getCurrentContentItem().setStringValue(chr);

					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("28", "PrimageNB", "Gain/loss"));

					std::string str_gain_loss = j["diagnosis"]["laboratory"]["molecular_studies"]["nca"]["alterations"][i]["grain_loss"]["value"];
					const char *gain_loss = str_gain_loss.c_str();
					doc->getTree().getCurrentContentItem().setStringValue(gain_loss);


					doc->getTree().goUp();

				}
			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));

			}
			//SCA
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("D020754", "MESH", "SCA"));
			bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["evaluated"]["yes"].get<bool>();
			bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["evaluated"]["no"].get<bool>();
			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));

				bool fish = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["method"]["fish"]["value"].get<bool>();
				bool snpa = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["method"]["snpa"]["value"].get<bool>();
				bool acgh = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["method"]["acgh"]["value"].get<bool>();
				bool icwgs = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["method"]["icwgs"]["value"].get<bool>();
				bool deepwgs = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["method"]["deepwgs"]["value"].get<bool>();

				if (fish == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("426329006", "SRT", "Method FISH"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (snpa == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("_DDB_G0285111", "PR", "Method SNPa"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (acgh == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("ERO_0001818", "ERO", "Method aCGH"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (icwgs == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("21", "PrimageNB", "Method IcWGS"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (deepwgs == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("22", "PrimageNB", "Method deepWGS"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}

				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("D020754", "MESH", "SCA Status"));
				bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["status"]["yes"].get<bool>();
				bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["status"]["no"].get<bool>();
				bool bool_no_result = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["status"]["no_result"].get<bool>();

				if (bool_yes == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("29", "PrimageNB", "SCA  Typical  1p-"));
					bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["minus1p"]["yes"].get<bool>();
					bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["minus1p"]["no"].get<bool>();
					bool_no_result = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["minus1p"]["no_result"].get<bool>();

					if (bool_yes == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

					}
					else if (bool_no == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

					}
					else if (bool_no_result == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LN", "No result"));

					}

					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("30", "PrimageNB", "SCA  Typical  +1q"));
					bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["plus1q"]["yes"].get<bool>();
					bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["plus1q"]["no"].get<bool>();
					bool_no_result = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["plus1q"]["no_result"].get<bool>();

					if (bool_yes == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

					}
					else if (bool_no == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

					}
					else if (bool_no_result == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LN", "No result"));

					}

					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("31", "PrimageNB", "SCA  Typical  +2p"));
					bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["plus2p"]["yes"].get<bool>();
					bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["plus2p"]["no"].get<bool>();
					bool_no_result = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["plus2p"]["no_result"].get<bool>();

					if (bool_yes == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

					}
					else if (bool_no == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

					}
					else if (bool_no_result == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LN", "No result"));

					}

					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("32", "PrimageNB", "SCA  Typical  3p-"));
					bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["minus3p"]["yes"].get<bool>();
					bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["minus3p"]["no"].get<bool>();
					bool_no_result = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["minus3p"]["no_result"].get<bool>();

					if (bool_yes == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

					}
					else if (bool_no == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

					}
					else if (bool_no_result == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LN", "No result"));

					}

					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("33", "PrimageNB", "SCA  Typical  4p-"));
					bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["minus4p"]["yes"].get<bool>();
					bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["minus4p"]["no"].get<bool>();
					bool_no_result = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["minus4p"]["no_result"].get<bool>();

					if (bool_yes == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

					}
					else if (bool_no == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

					}
					else if (bool_no_result == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LN", "No result"));

					}

					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("34", "PrimageNB", "SCA  Typical  +11q"));
					bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["plus11q"]["yes"].get<bool>();
					bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["plus11q"]["no"].get<bool>();
					bool_no_result = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["plus11q"]["no_result"].get<bool>();

					if (bool_yes == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

					}
					else if (bool_no == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

					}
					else if (bool_no_result == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LN", "No result"));

					}

					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("35", "PrimageNB", "SCA  Typical  11q-"));
					bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["minus11q"]["yes"].get<bool>();
					bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["minus11q"]["no"].get<bool>();
					bool_no_result = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["minus11q"]["no_result"].get<bool>();

					if (bool_yes == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

					}
					else if (bool_no == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

					}
					else if (bool_no_result == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LN", "No result"));

					}

					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("36", "PrimageNB", "SCA  Typical  +17q"));
					bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["plus17q"]["yes"].get<bool>();
					bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["plus17q"]["no"].get<bool>();
					bool_no_result = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["typical"]["plus17q"]["no_result"].get<bool>();

					if (bool_yes == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

					}
					else if (bool_no == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

					}
					else if (bool_no_result == true) {
						doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LN", "No result"));

					}

				}
				else if (bool_no == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

				}
				else if (bool_no_result == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LN", "No result"));

				}

				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("37", "PrimageNB", "SCA  Atypical"));
				bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["atypical"]["result"]["yes"].get<bool>();
				bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["atypical"]["result"]["no"].get<bool>();
				bool_no_result = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["atypical"]["result"]["no_result"].get<bool>();

				if (bool_yes == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("38", "PrimageNB", "SCA Number of alterations"));
					int num_alterations = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["atypical"]["number"]["value"].get<int>();
					std::string string_alterations = to_string(num_alterations);
					const char *alterations = string_alterations.c_str();
					doc->getTree().getCurrentContentItem().setStringValue(alterations);

					for (int i = 0; i < num_alterations; i++) {
						doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
						doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("39", "PrimageNB", "SCA Atypical Alteration"));
						doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
						doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("91272006", "SRT", "Chromosome"));

						std::string str_chr = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["atypical"]["alterations"][i]["chromosome"]["value"];
						const char *chr = str_chr.c_str();
						doc->getTree().getCurrentContentItem().setStringValue(chr);

						doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
						doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C13355", "NCIt", "Arm"));

						std::string str_arm = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["atypical"]["alterations"][i]["p_q"]["value"];
						const char *arm = str_arm.c_str();
						doc->getTree().getCurrentContentItem().setStringValue(arm);

						doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
						doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("28", "PrimageNB", "Gain/loss"));

						std::string str_gain_loss = j["diagnosis"]["laboratory"]["molecular_studies"]["sca"]["atypical"]["alterations"][i]["grain_loss"]["value"];
						const char *gain_loss = str_gain_loss.c_str();
						doc->getTree().getCurrentContentItem().setStringValue(gain_loss);


						doc->getTree().goUp();

					}


				}
				else if (bool_no == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

				}
				else if (bool_no_result == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LN", "No result"));

				}

			}

			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));

			}

			//ATRX deletion
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("OGG_3000000546", "OGG", "ATRX deletion"));
			bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["mutation_atrx"]["evaluated"]["yes"].get<bool>();
			bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["mutation_atrx"]["evaluated"]["no"].get<bool>();
			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				bool snpa = j["diagnosis"]["laboratory"]["molecular_studies"]["mutation_atrx"]["method"]["snpa"]["value"].get<bool>();
				bool wgs = j["diagnosis"]["laboratory"]["molecular_studies"]["mutation_atrx"]["method"]["deepwgs"]["value"].get<bool>();

				if (snpa == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("_DDB_G0285111", "PR", "Method SNPa"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (wgs == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("40", "PrimageNB", "Method WGS"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}

				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("OGG_3000000546", "OGG", "ATRX deletion Status"));
				bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["mutation_atrx"]["status"]["yes"].get<bool>();
				bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["mutation_atrx"]["status"]["no"].get<bool>();
				bool bool_no_result = j["diagnosis"]["laboratory"]["molecular_studies"]["mutation_atrx"]["status"]["no_result"].get<bool>();

				if (bool_yes == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

				}
				else if (bool_no == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

				}
				else if (bool_no_result == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LN", "No result"));

				}

			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));

			}

			//TERT
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("OGG_3000007015", "OGG", "Status TERT"));
			bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["tert"]["evaluated"]["yes"].get<bool>();
			bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["tert"]["evaluated"]["no"].get<bool>();
			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));

				bool fish = j["diagnosis"]["laboratory"]["molecular_studies"]["tert"]["method"]["fish"]["value"].get<bool>();
				bool snpa = j["diagnosis"]["laboratory"]["molecular_studies"]["tert"]["method"]["snpa"]["value"].get<bool>();
				bool acgh = j["diagnosis"]["laboratory"]["molecular_studies"]["tert"]["method"]["acgh"]["value"].get<bool>();
				bool icwgs = j["diagnosis"]["laboratory"]["molecular_studies"]["tert"]["method"]["icwgs"]["value"].get<bool>();
				bool deepwgs = j["diagnosis"]["laboratory"]["molecular_studies"]["tert"]["method"]["deepwgs"]["value"].get<bool>();

				if (fish == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("426329006", "SRT", "Method FISH"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (snpa == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("_DDB_G0285111", "PR", "Method SNPa"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (acgh == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("ERO_0001818", "ERO", "Method aCGH"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (icwgs == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("21", "PrimageNB", "Method IcWGS"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (deepwgs == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("22", "PrimageNB", "Method deepWGS"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}

				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("80", "PrimageNB", "Status TERT  Breakpoint/Rearrangement"));
				bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["tert"]["status"]["yes"].get<bool>();
				bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["tert"]["status"]["no"].get<bool>();
				bool bool_no_result = j["diagnosis"]["laboratory"]["molecular_studies"]["tert"]["status"]["no_result"].get<bool>();

				if (bool_yes == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

				}
				else if (bool_no == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

				}
				else if (bool_no_result == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LN", "No result"));

				}

			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));

			}
			
			//ALT
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("41", "PrimageNB", "Status ALT"));
			bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["tert"]["evaluated"]["yes"].get<bool>();
			bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["tert"]["evaluated"]["no"].get<bool>();
			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));

				bool pna_fish = j["diagnosis"]["laboratory"]["molecular_studies"]["alt"]["method"]["pna_fish"]["value"].get<bool>();
				bool pna_fish_pml = j["diagnosis"]["laboratory"]["molecular_studies"]["alt"]["method"]["pna_fish_pml"]["value"].get<bool>();
				bool circle_assay = j["diagnosis"]["laboratory"]["molecular_studies"]["alt"]["method"]["circle_assay"]["value"].get<bool>();
				

				if (pna_fish == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("42", "PrimageNB", "Method PNA tel FISH"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (pna_fish_pml == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("43", "PrimageNB", "Method PNA tel FISH + PML-body"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}
				if (circle_assay == true) {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C105650", "NCIt", "Method c-circle assay"));
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
				}

				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("41", "PrimageNB", "Status ALT"));
				bool_yes = j["diagnosis"]["laboratory"]["molecular_studies"]["alt"]["status"]["yes"].get<bool>();
				bool_no = j["diagnosis"]["laboratory"]["molecular_studies"]["alt"]["status"]["no"].get<bool>();
				bool bool_no_result = j["diagnosis"]["laboratory"]["molecular_studies"]["alt"]["status"]["no_result"].get<bool>();

				if (bool_yes == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

				}
				else if (bool_no == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

				}
				else if (bool_no_result == true) {
					doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA21136-9", "LN", "No result"));

				}


			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));

			}
		}
		else if (bool_no == true) {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));

		}


		doc->getTree().goUp();

		doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("GPOH-PID", "ACGT - MO", "GPOH gene expression classifier"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
		bool_yes = j["diagnosis"]["laboratory"]["gpoh_gene_expression_classifier"]["evaluated"]["yes"].get<bool>();
		bool_no = j["diagnosis"]["laboratory"]["gpoh_gene_expression_classifier"]["evaluated"]["no"].get<bool>();
		if (bool_yes == true) {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C25214", "NCIt", "Evaluated"));
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("394617004", "SRT", "Result"));
			std::string string_res = j["diagnosis"]["laboratory"]["gpoh_gene_expression_classifier"]["result"]["value"];
			if (string_res == "Favorable") {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C102560", "NCIt", "Favorable"));

			}
			else if (string_res == "Unfavorable") {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("C102561", "NCIt", "Unfavorable"));

			}
		}
		else if (bool_no == true) {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));

		}

		doc->getTree().goUp();

		doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("SWO_0000640", "SWO", "OMICS dataset"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("MVC_0000490", "MIXs", "OMICS dataset available"));
		bool_yes = j["diagnosis"]["laboratory"]["omics_dataset"]["available"]["yes"].get<bool>();
		bool_no = j["diagnosis"]["laboratory"]["omics_dataset"]["available"]["no"].get<bool>();
		if (bool_yes == true) {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("44", "PrimageNB", "Number of datasets available"));
			int num_dataset = j["diagnosis"]["laboratory"]["omics_dataset"]["number_datasets"]["value"].get<int>();
			std::string string_dataset = to_string(num_dataset);
			const char *dataset = string_dataset.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(dataset);

			for (int i = 0; i < num_dataset; i++) {		
				doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C47824", "NCIt", "Dataset"));
				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("45", "PrimageNB", "Type of analysis"));
				
				std::string str_type = j["diagnosis"]["laboratory"]["omics_dataset"]["datasets"][i]["analysis"]["value"];
				const char *type = str_type.c_str();
				doc->getTree().getCurrentContentItem().setStringValue(type);

				if (str_type == "Other") {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("74964007", "SRT", "Other"));
					std::string str_other_type = j["diagnosis"]["laboratory"]["omics_dataset"]["datasets"][i]["analysis_other"]["value"];
					const char *other_type = str_other_type.c_str();
					doc->getTree().getCurrentContentItem().setStringValue(other_type);
				}


				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("260686004", "SRT", "Method"));

				std::string str_method = j["diagnosis"]["laboratory"]["omics_dataset"]["datasets"][i]["method"]["value"];
				const char *method = str_method.c_str();
				doc->getTree().getCurrentContentItem().setStringValue(method);

				if (str_method == "Other") {
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("74964007", "SRT", "Other"));
					std::string str_other_method = j["diagnosis"]["laboratory"]["omics_dataset"]["datasets"][i]["method_other"]["value"];
					const char *other_method = str_other_method.c_str();
					doc->getTree().getCurrentContentItem().setStringValue(other_method);
				}



				doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
				doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C25402", "NCIt", "Accession number or link"));

				bool_yes = j["diagnosis"]["laboratory"]["omics_dataset"]["datasets"][i]["accesion_number"]["result"]["yes"].get<bool>();
				bool_no = j["diagnosis"]["laboratory"]["omics_dataset"]["datasets"][i]["accesion_number"]["result"]["no"].get<bool>();
				
				if (bool_yes == true) {
					doc->getTree().getCurrentContentItem().setStringValue("Yes");
					doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
					doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C25402", "NCIt", "Number/Link"));
					std::string str_number = j["diagnosis"]["laboratory"]["omics_dataset"]["datasets"][i]["accesion_number"]["number"]["value"];
					const char *number = str_number.c_str();
					doc->getTree().getCurrentContentItem().setStringValue(number);

					if (str_number == "Other") {
						doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
						doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("74964007", "SRT", "Other"));
						std::string str_o_number = j["diagnosis"]["laboratory"]["omics_dataset"]["datasets"][i]["accesion_number"]["number_other"]["value"];
						const char *o_number = str_o_number.c_str();
						doc->getTree().getCurrentContentItem().setStringValue(o_number);
					}
					
					
				}
				else if (bool_no == true) {
					doc->getTree().getCurrentContentItem().setStringValue("No");
				}

				doc->getTree().goUp();

			}
		}
		else if (bool_no == true) {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

		}

		//############################       END  LABORATORY #############################

		//############################       START  IMAGING #############################
doc->getTree().goUp();
doc->getTree().goUp();

doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("363679005", "SRT", "Imaging"));
doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C17059", "NCIt", "Radiology"));
doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID10813", "RADLEX", "MRI"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID10813", "RADLEX", "MRI"));
bool_yes = j["diagnosis"]["imaging"]["radiology"]["mri"]["evaluated"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["radiology"]["mri"]["evaluated"]["no"].get<bool>();
bool not_available = j["diagnosis"]["imaging"]["radiology"]["mri"]["evaluated"]["not_available"].get<bool>();
bool missing = j["diagnosis"]["imaging"]["radiology"]["mri"]["evaluated"]["missing"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385658003", "SRT", "Done"));
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("706821004", "SRT", "Manufacturer and model"));
	std::string string_model = j["diagnosis"]["imaging"]["radiology"]["mri"]["equipment"]["value"];
	const char *model = string_model.c_str();
	doc->getTree().getCurrentContentItem().setStringValue(model);
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_DateTime, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("410672004", "SRT", "Date"));
	std::string string_date = j["diagnosis"]["imaging"]["radiology"]["mri"]["date"]["value"];
	std::string chars = "-T:.Z";
	for (char c : chars) {
		string_date.erase(std::remove(string_date.begin(), string_date.end(), c), string_date.end());
	}
	std::string sub_string_date = string_date.substr(0, 12);
	const char *date = sub_string_date.c_str();
	doc->getTree().getCurrentContentItem().setObservationDateTime(date);
	doc->getTree().getCurrentContentItem().setStringValue(date);



}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385660001", "SRT", "Not done"));
}
else if (not_available == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA7338-2", "LN", "Not available"));
}
else if (missing == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA14698-7", "LN", "Missing"));
}

doc->getTree().goUp();
doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID10321", "RADLEX", "CT"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID10321", "RADLEX", "CT"));
bool_yes = j["diagnosis"]["imaging"]["radiology"]["ct"]["evaluated"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["radiology"]["ct"]["evaluated"]["no"].get<bool>();
not_available = j["diagnosis"]["imaging"]["radiology"]["ct"]["evaluated"]["not_available"].get<bool>();
missing = j["diagnosis"]["imaging"]["radiology"]["ct"]["evaluated"]["missing"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385658003", "SRT", "Done"));
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("706821004", "SRT", "Manufacturer and model"));
	std::string string_model = j["diagnosis"]["imaging"]["radiology"]["ct"]["equipment"]["value"];
	const char *model = string_model.c_str();
	doc->getTree().getCurrentContentItem().setStringValue(model);
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("410672004", "SRT", "Date"));
	std::string string_date = j["diagnosis"]["imaging"]["radiology"]["ct"]["date"]["value"];
	const char *date = string_date.c_str();
	doc->getTree().getCurrentContentItem().setStringValue(date);

}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385660001", "SRT", "Not done"));
}
else if (not_available == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA7338-2", "LN", "Not available"));
}
else if (missing == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA14698-7", "LN", "Missing"));
}

doc->getTree().goUp();
doc->getTree().goUp();
doc->getTree().goUp();
doc->getTree().goUp();


doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C16922", "NCIt", "Nuclear medicine"));
doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID45807", "RADLEX", "MIBG entire body"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID45807", "RADLEX", "MIBG entire body"));
bool_yes = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["evaluated"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["evaluated"]["no"].get<bool>();
not_available = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["evaluated"]["not_available"].get<bool>();
missing = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["evaluated"]["missing"].get<bool>();
if (bool_yes == true) 
	{
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385658003", "SRT", "Done"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("706821004", "SRT", "Manufacturer and model"));
		std::string string_model = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["equipment"]["value"];
		const char *model = string_model.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(model);

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_DateTime, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("410672004", "SRT", "Date"));
		std::string string_date = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["date"]["value"];
		std::string chars = "-T:.Z";
		for (char c : chars) {
			string_date.erase(std::remove(string_date.begin(), string_date.end(), c), string_date.end());
		}
		std::string sub_string_date = string_date.substr(0, 12);
		const char *date = sub_string_date.c_str();
		doc->getTree().getCurrentContentItem().setObservationDateTime(date);
		doc->getTree().getCurrentContentItem().setStringValue(date);


		/*const char *date = string_date.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(date);*/

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("272394005", "SRT", "Technique"));
		std::string string_technique = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["technique"]["value"];
		const char *technique = string_technique.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(technique);

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("408102007", "SRT", "Dose unit"));
		std::string string_dose_unit = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["dose_unit"]["value"];
		const char *dose_unit = string_dose_unit.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(dose_unit);

		std::string unknown = "Unknown";
		if (dose_unit != unknown) {
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("OBI_0002491", "OBI", "Dose value"));
			int num_dose = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["dose"]["value"].get<int>();
			std::string string_dose = to_string(num_dose);
			const char *dose = string_dose.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(dose);
		}

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C43385", "NCIt", "Image acquisition"));
		bool bool_other = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["image_acquisition"]["other"].get<bool>();
		bool bool_4h = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["image_acquisition"]["other"].get<bool>();
		//bool bool_24h = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["image_acquisition"]["other"].get<bool>();


		if (bool_other == true) {
			doc->getTree().getCurrentContentItem().setStringValue("Other");
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("130194", "DCM", "Time after injection in h"));
			int num_time_after = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["image_acquisition_other"]["value"].get<int>();
			std::string string_time_after = to_string(num_time_after);
			const char *time_after = string_time_after.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(time_after);

		}
		else if (bool_4h == true) {
			doc->getTree().getCurrentContentItem().setStringValue("4h");
		}
		else {
			doc->getTree().getCurrentContentItem().setStringValue("24h");
		}
		
		doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C71688", "NCIt", "Uptake"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("MTHU010322", "LN", "Primary site"));
std::string	mibg_primary = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["uptake"]["primary_site"]["value"];

if (mibg_primary == "Yes") {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (mibg_primary == "No") {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (mibg_primary == "Not evaluable") {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
}


doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("fma303630", "FMA", "Skeleton"));
std::string mibg_skeleton = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["uptake"]["skeleton"]["value"];

if (mibg_skeleton == "Yes") {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (mibg_skeleton == "No") {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (mibg_skeleton == "Not evaluable") {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C12471", "NCIt", "Soft tissue"));
std::string mibg_soft = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["uptake"]["soft_tissue"]["value"];

if (mibg_soft == "Yes") {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (mibg_soft == "No") {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (mibg_soft == "Not evaluable") {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("fma7197", "FMA", "Liver"));
std::string mibg_liver = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["uptake"]["liver"]["value"];
if (mibg_liver == "Yes") {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (mibg_liver == "No") {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (mibg_liver == "Not evaluable") {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID34290", "RADLEX", "Skin"));
std::string mibg_skin = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["uptake"]["skin"]["value"];

if (mibg_skin == "Yes") {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (mibg_skin == "No") {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (mibg_skin == "Not evaluable") {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("46", "PrimageNB", "Score SIOPEN"));
/*bool_yes = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["score_siopen"]["result"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["score_siopen"]["result"]["no"].get<bool>();
if (bool_yes == true) {
	int num_score_siopen = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["score_siopen"]["value"]["value"].get<int>;
	std::string string_score_siopen = to_string(num_score_siopen);
	const char *score_siopen = string_score_siopen.c_str();
	doc->getTree().getCurrentContentItem().setStringValue(score_siopen);

}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setStringValue("No");
}*/


checker = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["result"]["value"].is_null();
if (checker == false) {
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("394617004", "SRT", "Result"));
	std::string string_result = j["diagnosis"]["imaging"]["nuclear_medicine"]["mibg"]["result"]["value"];
	const char *result = string_result.c_str();
	doc->getTree().getCurrentContentItem().setStringValue(result);
}

	}
else if (not_available == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA7338-2", "LN", "Not available"));
}
else {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA14698-7", "LN", "Missing"));
}

doc->getTree().goUp();
doc->getTree().goUp();

doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID10341", "RADLEX", "PET/CT"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID10341", "RADLEX", "PET/CT"));
bool_yes = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["evaluated"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["evaluated"]["no"].get<bool>();
not_available = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["evaluated"]["not_available"].get<bool>();
missing = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["evaluated"]["missing"].get<bool>();
if (bool_yes == true) {

	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385658003", "SRT", "Done"));
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("706821004", "SRT", "Manufacturer and model"));
	checker = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["equipment"]["value"].is_null();
	std::string string_model = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["equipment"]["value"];
	const char *model = string_model.c_str();
	doc->getTree().getCurrentContentItem().setStringValue(model);

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("410672004", "SRT", "Date"));
	std::string string_date = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["date"]["value"];
	const char *date = string_date.c_str();
	doc->getTree().getCurrentContentItem().setStringValue(date);

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("35321007", "SRT", "FDG - 18F(administered dose)"));
	int number_fdg_18f_dose = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["fdg_18f"]["value"].get<int>();
	std::string string_number_fdg_18f_dose = to_string(number_fdg_18f_dose);
	const char *fdg_18f_dose = string_number_fdg_18f_dose.c_str();
	doc->getTree().getCurrentContentItem().setStringValue(fdg_18f_dose);
	
	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("MTHU010322", "NCIt", "Uptake"));
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("MTHU010322", "LN", "Primary site"));
	std::string pet_primary = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["uptake"]["primary_site"]["value"];
	if (pet_primary == "Yes") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("276625007", "SRT", "Primary site - Level"));
		std::string str_pet_site_level = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["uptake"]["primary_site_level"]["value"];
		const char *pet_site_level = str_pet_site_level.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(pet_site_level);

		/*if (pet_site_level == "Yes") {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
		}
		else if (pet_site_level == "No") {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
		}
		else if (pet_site_level == "Not evaluable") {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
		}*/
	}
	
	
	else if (pet_primary == "No") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (pet_primary == "Not evaluable") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
	}
	
	
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID5231", "RADLEX", "Skeleton metastasis"));
	std::string pet_skeleton = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["uptake"]["metastasis_skeleton"]["value"];
	if (pet_skeleton == "Yes") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("276625007", "SRT", "Skeleton metastasis - Level"));
		std::string str_pet_skeleton_level = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["uptake"]["metastasis_skeleton_level"]["value"];
		const char *pet_skeleton_level = str_pet_skeleton_level.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(pet_skeleton_level);
		/*bool_yes = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["uptake"]["metastasis_skeleton_level"]["yes"].get<bool>();
		bool_no = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["uptake"]["metastasis_skeleton_level"]["no"].get<bool>();
		bool_not_evaluable = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["uptake"]["metastasis_skeleton_level"]["not_evaluable"].get<bool>();
		if (bool_yes == true) {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
		}
		else if (bool_no == true) {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
		}
		else if (bool_not_evaluable == true) {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
		}*/
	}
	else if (pet_skeleton == "No") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (pet_skeleton == "Not evaluable") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
	}
	
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID5231", "RADLEX", "Soft tissue metastasis"));
	std::string pet_soft = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["uptake"]["metastasis_soft_tissue"]["value"];

	if (pet_soft == "Yes") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("276625007", "SRT", "Soft tissue metastasis - Level"));
		std:: string str_pet_soft_level = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["uptake"]["metastasis_soft_tissue_level"]["value"];
		const char *pet_soft_level = str_pet_soft_level.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(pet_soft_level);
		/*bool_no = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["uptake"]["metastasis_soft_tissue_level"]["no"].get<bool>();
		bool_not_evaluable = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["uptake"]["metastasis_soft_tissue_level"]["not_evaluable"].get<bool>();
		if (bool_yes == true) {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
		}
		else if (bool_no == true) {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
		}
		else if (bool_not_evaluable == true) {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
		}
		*/
		


	}
	else if (pet_soft == "No") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (pet_soft == "Not evaluable") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
	}
	
	checker = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["result"]["value"].is_null();
	if (checker == false) {
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("394617004", "SRT", "Result"));
		std::string string_result = j["diagnosis"]["imaging"]["nuclear_medicine"]["pet"]["result"]["value"];
		const char *result = string_result.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(result);
	}

}
	

else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385660001", "SRT", "Not done"));
}
else if (not_available == true) {

	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA7338-2", "LN", "Not available"));
}
else if (missing == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA14698-7", "LN", "Missing"));
}

doc->getTree().goUp();
doc->getTree().goUp();
doc->getTree().goUp();

doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("201006093642984336", "IOBC", "Imaging defined risk factors"));


doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("47", "PrimageNB", "Ipsilateral tumor exension"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("MTHU059898", "LN", "Neck/chest"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["ipsilateral_tumor_extension"]["neck_chest"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["ipsilateral_tumor_extension"]["neck_chest"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["ipsilateral_tumor_extension"]["neck_chest"]["unknown"].get<bool>();
bool bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["ipsilateral_tumor_extension"]["neck_chest"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("LP264363-5", "LN", "Chest/abdomed"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["ipsilateral_tumor_extension"]["chest_abdomen"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["ipsilateral_tumor_extension"]["chest_abdomen"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["ipsilateral_tumor_extension"]["chest_abdomen"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["ipsilateral_tumor_extension"]["chest_abdomen"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("LP264569-7", "LN", "Abdomen/Pelvis"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["ipsilateral_tumor_extension"]["abdomen_pelvis"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["ipsilateral_tumor_extension"]["abdomen_pelvis"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["ipsilateral_tumor_extension"]["abdomen_pelvis"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["ipsilateral_tumor_extension"]["abdomen_pelvis"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}
doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("fma7155", "FMA", "Neck"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("48", "PrimageNB", "Tumor encasing carotid and/or vertebral artery and/or internal jugular vein"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["neck"]["carotid_vertebral_artery_internal_jugular_vein"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["neck"]["carotid_vertebral_artery_internal_jugular_vein"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["neck"]["carotid_vertebral_artery_internal_jugular_vein"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["neck"]["carotid_vertebral_artery_internal_jugular_vein"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("49", "PrimageNB", "Tumor extending to base of skull"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["neck"]["base_of_skull"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["neck"]["base_of_skull"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["neck"]["base_of_skull"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["neck"]["base_of_skull"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("50", "PrimageNB", "Tumor compressing the trachea"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["neck"]["trachea"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["neck"]["trachea"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["neck"]["trachea"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["neck"]["trachea"]["not_applicable"].get<bool>();

if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}

doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("51", "PrimageNB", "Cervico thoracic junction"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("52", "PrimageNB", "Tumor encasing brachial plexus roots"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["cervico_thoracic_junction"]["brachial_plexus_roots"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["cervico_thoracic_junction"]["brachial_plexus_roots"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["cervico_thoracic_junction"]["brachial_plexus_roots"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["cervico_thoracic_junction"]["brachial_plexus_roots"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("53", "PrimageNB", "Tumor encasing subclavial vessels and/or vertebral and/or carotid artery"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["cervico_thoracic_junction"]["subclavian_vessels_vertebral_carotid_artery"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["cervico_thoracic_junction"]["subclavian_vessels_vertebral_carotid_artery"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["cervico_thoracic_junction"]["subclavian_vessels_vertebral_carotid_artery"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["cervico_thoracic_junction"]["subclavian_vessels_vertebral_carotid_artery"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("50", "PrimageNB", "Tumor compressing the trachea"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["cervico_thoracic_junction"]["trachea"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["cervico_thoracic_junction"]["trachea"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["cervico_thoracic_junction"]["trachea"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["cervico_thoracic_junction"]["trachea"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}

doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("fma9576", "FMA", "Thorax"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("54", "PrimageNB", "Tumor encasing the aorta and/or major branches"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["thorax"]["aorta_major_branches"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["thorax"]["aorta_major_branches"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["thorax"]["aorta_major_branches"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["thorax"]["aorta_major_branches"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("55", "PrimageNB", "Tumor compressing the trachea and/or principal bronchi"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["thorax"]["trachea_principal_bronchi"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["thorax"]["trachea_principal_bronchi"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["thorax"]["trachea_principal_bronchi"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["thorax"]["trachea_principal_bronchi"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}


doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("56", "PrimageNB", "Lower mediastinal tumor, infiltrating the costovertebral juntion T9 and T12"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["thorax"]["lower_mediastinal_tumor"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["thorax"]["lower_mediastinal_tumor"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["thorax"]["lower_mediastinal_tumor"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["thorax"]["lower_mediastinal_tumor"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}

/*
doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("LN:MTHU021540", OFFIS_CODING_SCHEME_DESIGNATOR, "Toraco abdominal"));
*/

doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("LP264569-7", "LN", "Abdomen/Pelvis"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("57", "PrimageNB", "Tumor infiltrating the porta hepatis and/or the hepatoduodenal ligament"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["porta_hepatoduodenal_ligament"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["porta_hepatoduodenal_ligament"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["porta_hepatoduodenal_ligament"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["porta_hepatoduodenal_ligament"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("58", "PrimageNB", "Tumor encasing branches of the superior mesenteric artery at the mesenteric root"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["mesenteric_artery"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["mesenteric_artery"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["mesenteric_artery"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["mesenteric_artery"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("59", "PrimageNB", "Tumor encasing the origin of the celiac axis and/or of the superior mesenteric artery"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["celiac_axis_superior_mesenteric_artery"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["celiac_axis_superior_mesenteric_artery"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["celiac_axis_superior_mesenteric_artery"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["celiac_axis_superior_mesenteric_artery"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("60", "PrimageNB", "Tumor invading one or both renal pedicles"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["renal_pedicles"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["renal_pedicles"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["renal_pedicles"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["renal_pedicles"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("61", "PrimageNB", "Tumor encasing the aorta and/or vena cava"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["aorta_vena_cava"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["aorta_vena_cava"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["aorta_vena_cava"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["aorta_vena_cava"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("62", "PrimageNB", "Tumor encasing the iliac vessels"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["iliac_vessels"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["iliac_vessels"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["iliac_vessels"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["iliac_vessels"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("63", "PrimageNB", "Pelvic tumor crossing the sciatic notch"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["sciatic_notch"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["sciatic_notch"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["sciatic_notch"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["abdomen_pelvis"]["sciatic_notch"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}

doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("360179007", "LN", "Intraspinal"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("64", "PrimageNB", "Intraspinal tumor extension whatever the location provided that more than 1/3 of the spinal canal in the axial plane is invaded and/or the perimedullary leptomeningeal spaces are not visible and/or the spinal cord signal is abnormal	"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["intraspinal"]["invasion"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["intraspinal"]["invasion"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["intraspinal"]["invasion"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["intraspinal"]["invasion"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}
//1510
doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("65", "PrimageNB", "Infiltration adjacent organs"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("66", "PrimageNB", "Infiltration of adjacent organs/structures – Pericardium, ?diaphragm, kidney, liver, duodenopancreatic block and mesentery"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["infiltration_adjacent_organs"]["infiltration"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["infiltration_adjacent_organs"]["infiltration"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["infiltration_adjacent_organs"]["infiltration"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["infiltration_adjacent_organs"]["infiltration"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}

doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("67", "PrimageNB", "Conditions to be recorded but not considered IDRFs"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("68", "PrimageNB", "Multifocal primary tumours"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["conditions"]["multifocal_primary_tumors"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["conditions"]["multifocal_primary_tumors"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["conditions"]["multifocal_primary_tumors"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["conditions"]["multifocal_primary_tumors"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("60046008", "SRT", "Pleural effusion"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["conditions"]["pleural_effusion"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["conditions"]["pleural_effusion"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["conditions"]["pleural_effusion"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["conditions"]["pleural_effusion"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("389026000", "SRT", "Ascites"));
bool_yes = j["diagnosis"]["imaging"]["idrf"]["conditions"]["ascites"]["yes"].get<bool>();
bool_no = j["diagnosis"]["imaging"]["idrf"]["conditions"]["ascites"]["no"].get<bool>();
bool_unknown = j["diagnosis"]["imaging"]["idrf"]["conditions"]["ascites"]["unknown"].get<bool>();
bool_not_applicable = j["diagnosis"]["imaging"]["idrf"]["conditions"]["ascites"]["not_applicable"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}
else if (bool_unknown == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
}
else if (bool_not_applicable == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385432009", "SRT", "Not applicable"));
}

doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("394617004", "SRT", "Result"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("394617004", "SRT", "Result"));
std::string string_result_2 = j["diagnosis"]["imaging"]["idrf"]["result"]["value"];
const char *result_2 = string_result_2.c_str();
doc->getTree().getCurrentContentItem().setStringValue(result_2);

//############################       END  IMAGING #############################//

//############################       STAGING #############################//
doc->getTree().goUp();
doc->getTree().goUp();


doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C15608", "NCIt", "Staging"));
doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("399687005", "SRT", "Primary tumor site"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("277112006", "SRT", "Abdominal adrenal"));
bool_yes = j["diagnosis"]["staging"]["primary_tumor_site"]["abdominal_adrenal"]["yes"].get<bool>();
bool_no = j["diagnosis"]["staging"]["primary_tumor_site"]["abdominal_adrenal"]["no"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}


doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("74964007", "SRT", "Abdominal other"));
bool_yes = j["diagnosis"]["staging"]["primary_tumor_site"]["abdominal_other"]["yes"].get<bool>();
bool_no = j["diagnosis"]["staging"]["primary_tumor_site"]["abdominal_other"]["no"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("261064006", "SRT", "Cervical"));
bool_yes = j["diagnosis"]["staging"]["primary_tumor_site"]["cervical"]["yes"].get<bool>();
bool_no = j["diagnosis"]["staging"]["primary_tumor_site"]["cervical"]["no"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("fma9578", "FMA", "Pelvic"));
bool_yes = j["diagnosis"]["staging"]["primary_tumor_site"]["pelvic"]["yes"].get<bool>();
bool_no = j["diagnosis"]["staging"]["primary_tumor_site"]["pelvic"]["no"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("261179002", "SRT", "Thoracic"));
bool_yes = j["diagnosis"]["staging"]["primary_tumor_site"]["thoracic"]["yes"].get<bool>();
bool_no = j["diagnosis"]["staging"]["primary_tumor_site"]["thoracic"]["no"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
bool_yes = j["diagnosis"]["staging"]["primary_tumor_site"]["unknown"]["yes"].get<bool>();
bool_no = j["diagnosis"]["staging"]["primary_tumor_site"]["unknown"]["no"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("74964007", "SRT", "Others"));
bool_yes = j["diagnosis"]["staging"]["primary_tumor_site"]["others"]["result"]["yes"].get<bool>();
bool_no = j["diagnosis"]["staging"]["primary_tumor_site"]["others"]["result"]["no"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setStringValue("Yes");
	std::string string_value = j["diagnosis"]["staging"]["primary_tumor_site"]["others"]["value"]["value"];
	const char *value = string_value.c_str();
	doc->getTree().getCurrentContentItem().setStringValue(value);

}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setStringValue("No");
}
///////////
doc->getTree().goUp();
doc->getTree().goUp();

doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID5231", "RADLEX", "Metastasis"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID5231", "RADLEX", "Metastasis"));
bool_yes = j["diagnosis"]["staging"]["metastasis"]["result"]["yes"].get<bool>();
bool_no = j["diagnosis"]["staging"]["metastasis"]["result"]["no"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("fma30317", "FMA", "Bone"));
	bool_yes = j["diagnosis"]["staging"]["metastasis"]["bone"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["staging"]["metastasis"]["bone"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["staging"]["metastasis"]["bone"]["unknown"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID38594", "RADLEX", "Bone marrow"));
	bool_yes = j["diagnosis"]["staging"]["metastasis"]["bone_marrow"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["staging"]["metastasis"]["bone_marrow"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["staging"]["metastasis"]["bone_marrow"]["unknown"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("fma7197", "FMA", "Liver"));
	bool_yes = j["diagnosis"]["staging"]["metastasis"]["liver"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["staging"]["metastasis"]["liver"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["staging"]["metastasis"]["liver"]["unknown"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("D008198", "MESH", "Lymph nodes"));
	bool_yes = j["diagnosis"]["staging"]["metastasis"]["lymph_nodes"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["staging"]["metastasis"]["lymph_nodes"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["staging"]["metastasis"]["lymph_nodes"]["unknown"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID34290", "RADLEX", "Skin"));
	bool_yes = j["diagnosis"]["staging"]["metastasis"]["skin"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["staging"]["metastasis"]["skin"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["staging"]["metastasis"]["skin"]["unknown"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("69", "PrimageNB", "Orbita"));
	bool_yes = j["diagnosis"]["staging"]["metastasis"]["orbita"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["staging"]["metastasis"]["orbita"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["staging"]["metastasis"]["orbita"]["unknown"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("261731003", "SRT", "CNS"));
	bool_yes = j["diagnosis"]["staging"]["metastasis"]["cns"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["staging"]["metastasis"]["cns"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["staging"]["metastasis"]["cns"]["unknown"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID13437", "RADLEX", "Lungs"));
	bool_yes = j["diagnosis"]["staging"]["metastasis"]["lungs"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["staging"]["metastasis"]["lungs"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["staging"]["metastasis"]["lungs"]["unknown"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
	}

	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("74964007", "SRT", "Others"));
	bool_yes = j["diagnosis"]["staging"]["metastasis"]["other"]["result"]["yes"].get<bool>();
	bool_no = j["diagnosis"]["staging"]["metastasis"]["other"]["result"]["no"].get<bool>();
	bool_unknown = j["diagnosis"]["staging"]["metastasis"]["other"]["result"]["unknown"].get<bool>();
	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setStringValue("Yes");
		std::string string_value = j["diagnosis"]["staging"]["metastasis"]["other"]["value"]["value"];
		const char *value = string_value.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(value);

	}
	
	else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setStringValue("No");
	}

}

doc->getTree().goUp();
doc->getTree().goUp();

doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C15608", "NCIt", "Staging"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("CCONT_0000062", "CCO", "Risk group - INRG"));
std::string string_riskg = j["diagnosis"]["staging"]["staging"]["risk_group_inrg"]["value"];
const char *riskg = string_riskg.c_str();
doc->getTree().getCurrentContentItem().setStringValue(riskg);

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("409720004", "SRT", "Risk group - Database"));
std::string string_risk_db = j["diagnosis"]["staging"]["staging"]["risk_group_database"]["value"];
const char *risk_db = string_risk_db.c_str();
doc->getTree().getCurrentContentItem().setStringValue(risk_db);

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("409720004", "SRT", "INSS"));
std::string string_inss = j["diagnosis"]["staging"]["staging"]["inss"]["value"];
const char *inss = string_inss.c_str();
doc->getTree().getCurrentContentItem().setStringValue(inss);

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C133427", "NCIt", "INRGSS"));
std::string string_inrgss = j["diagnosis"]["staging"]["staging"]["inrgss"]["value"];
const char *inrgss = string_inrgss.c_str();
doc->getTree().getCurrentContentItem().setStringValue(inrgss);

//############################       END STAGING #############################//

	//############################       TREATMENT AND EVALUATION #############################//


doc->getTree().gotoNamedNode(DSRCodedEntryValue("1", "PrimageNB", "Neuroblastoma structured report"));

doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C15839", "NCIt", "Treatment and evaluation"));
doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("708255002", "SRT", "First treatment"));

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("110465008", "SRT", "Clinical trial"));
bool_yes = j["treatment_and_evaluation"]["first_treatment"]["clinical_trial"]["result"]["yes"].get<bool>();
bool_no = j["treatment_and_evaluation"]["first_treatment"]["clinical_trial"]["result"]["no"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setStringValue("Yes");
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C42614", "NCIt", "Name"));
	std::string string_name = j["treatment_and_evaluation"]["first_treatment"]["clinical_trial"]["value"]["value"];
	const char *name = string_name.c_str();
	doc->getTree().getCurrentContentItem().setStringValue(name);
	std::string aux = "LINES";
	if (name == aux) {

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("70", "PrimageNB", "LINES group"));
		int num_lines_group = j["treatment_and_evaluation"]["first_treatment"]["lines_group"]["value"].get<int>();
		std::string string_lines_group = to_string(num_lines_group);
		const char *lines_group = string_lines_group.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(lines_group);

	}

}
else if (bool_no) {
	doc->getTree().getCurrentContentItem().setStringValue("No");
}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID8", "RADLEX", "Type of treatment"));
std::string string_typeT = j["treatment_and_evaluation"]["first_treatment"]["type_of_treatment"]["value"];
const char *typeT = string_typeT.c_str();
doc->getTree().getCurrentContentItem().setStringValue(typeT);
std::string surgery = "Surgery";
std::string chemo_surgery = "Chemotherapy and surgery";
if (typeT == surgery || typeT == chemo_surgery) {
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_DateTime, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("OperationDate", "SDO", "Date of surgery"));
	std::string string_s_date = j["treatment_and_evaluation"]["first_treatment"]["surgery_date"]["value"];
	std::string chars = "-T:.Z";
	for (char c : chars) {
		string_s_date.erase(std::remove(string_s_date.begin(), string_s_date.end(), c), string_s_date.end());
	}
	std::string sub_string_s_date = string_s_date.substr(0, 12);
	const char *s_date = sub_string_s_date.c_str();
	doc->getTree().getCurrentContentItem().setObservationDateTime(s_date);
	doc->getTree().getCurrentContentItem().setStringValue(s_date);

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_DateTime, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("LP249274-4", "LN", "Date of chemotherapy initiation"));
	std::string string_i_date = j["treatment_and_evaluation"]["first_treatment"]["initiation_date"]["value"];
	chars = "-T:.Z";
	for (char c : chars) {
		string_i_date.erase(std::remove(string_i_date.begin(), string_i_date.end(), c), string_i_date.end());
	}
	std::string sub_string_i_date = string_i_date.substr(0, 12);
	const char *i_date = sub_string_i_date.c_str();
	doc->getTree().getCurrentContentItem().setObservationDateTime(i_date);
	doc->getTree().getCurrentContentItem().setStringValue(i_date);

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_DateTime, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("71", "PrimageNB", "End date of chemotherapy"));
	std::string string_e_date = j["treatment_and_evaluation"]["first_treatment"]["end_date"]["value"];
	chars = "-T:.Z";
	for (char c : chars) {
		string_e_date.erase(std::remove(string_e_date.begin(), string_e_date.end(), c), string_e_date.end());
	}
	std::string sub_string_e_date = string_e_date.substr(0, 12);
	const char *e_date = sub_string_e_date.c_str();
	doc->getTree().getCurrentContentItem().setObservationDateTime(e_date);
	doc->getTree().getCurrentContentItem().setStringValue(e_date);

	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("200906063214672861", "IOBC", "VP/Carbo"));
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("200906063214672861", "IOBC", "VP/Carbo"));
	bool_yes = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["vp_carbo"]["result"]["yes"].get<bool>();
	bool_no = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["vp_carbo"]["result"]["no"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setStringValue("Yes");
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("72", "PrimageNB", "Number of cycles"));
		int num_number_cycles = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["vp_carbo"]["number_of_cycles"]["value"].get<int>();
		std::string string_number_cycles = to_string(num_number_cycles);
		const char *number_cycles = string_number_cycles.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(number_cycles);
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setStringValue("No");
	}

	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("73", "PrimageNB", "CADO"));
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("LN:MTHU021540", "PrimageNB", "CADO"));
	bool_yes = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["cado"]["result"]["yes"].get<bool>();
	bool_no = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["cado"]["result"]["no"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("72", "PrimageNB", "Number of cycles"));
		int num_number_cycles = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["cado"]["number_of_cycles"]["value"].get<int>();
		std::string string_number_cycles = to_string(num_number_cycles);
		const char *number_cycles = string_number_cycles.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(number_cycles);
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("SRT:439401001", "PrimageNB", "CO"));
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("LN:MTHU021540", "PrimageNB", "CO"));
	bool_yes = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["co"]["result"]["yes"].get<bool>();
	bool_no = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["co"]["result"]["no"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("72", "PrimageNB", "Number of cycles"));
		int num_number_cycles = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["co"]["number_of_cycles"]["value"].get<int>();
		std::string string_number_cycles = to_string(num_number_cycles);
		const char *number_cycles = string_number_cycles.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(number_cycles);
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("SRT:439401001", "PrimageNB", "Rapid COJEC"));
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("LN:MTHU021540", "PrimageNB", "Rapid COJEC"));
	bool_yes = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["rapid_cojec"]["result"]["yes"].get<bool>();
	bool_no = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["rapid_cojec"]["result"]["no"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("72", "PrimageNB", "Number of cycles"));
		int num_number_cycles = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["rapid_cojec"]["number_of_cycles"]["value"].get<int>();
		std::string string_number_cycles = to_string(num_number_cycles);
		const char *number_cycles = string_number_cycles.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(number_cycles);
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("81", "PrimageNB", "N4"));

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("81", "PrimageNB", "N4"));
	bool_yes = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["n4"]["result"]["yes"].get<bool>();
	bool_no = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["n4"]["result"]["no"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("72", "PrimageNB", "Number of cycles"));
		int num_number_cycles = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["n7"]["number_of_cycles"]["value"].get<int>();
		std::string string_number_cycles = to_string(num_number_cycles);
		const char *number_cycles = string_number_cycles.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(number_cycles);
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("82", "PrimageNB", "N5"));

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("82", "PrimageNB", "N5"));
	bool_yes = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["n5"]["result"]["yes"].get<bool>();
	bool_no = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["n5"]["result"]["no"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("72", "PrimageNB", "Number of cycles"));
		int num_number_cycles = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["n5"]["number_of_cycles"]["value"].get<int>();
		std::string string_number_cycles = to_string(num_number_cycles);
		const char *number_cycles = string_number_cycles.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(number_cycles);
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("83", "PrimageNB", "N6"));

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("83", "PrimageNB", "N6"));
	bool_yes = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["n6"]["result"]["yes"].get<bool>();
	bool_no = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["n6"]["result"]["no"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("72", "PrimageNB", "Number of cycles"));
		int num_number_cycles = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["n6"]["number_of_cycles"]["value"].get<int>();
		std::string string_number_cycles = to_string(num_number_cycles);
		const char *number_cycles = string_number_cycles.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(number_cycles);
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}



	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("84", "PrimageNB", "N7"));

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("84", "PrimageNB", "N7"));
	bool_yes = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["n7"]["result"]["yes"].get<bool>();
	bool_no = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["n7"]["result"]["no"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("72", "PrimageNB", "Number of cycles"));
		int num_number_cycles = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["n7"]["number_of_cycles"]["value"].get<int>();
		std::string string_number_cycles = to_string(num_number_cycles);
		const char *number_cycles = string_number_cycles.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(number_cycles);
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("85", "PrimageNB", "Modified N7"));

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("85", "PrimageNB", "Modified N7"));
	bool_yes = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["n7"]["result"]["yes"].get<bool>();
	bool_no = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["n7"]["result"]["no"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("72", "PrimageNB", "Number of cycles"));
		int num_number_cycles = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["modified_n7"]["number_of_cycles"]["value"].get<int>();
		std::string string_number_cycles = to_string(num_number_cycles);
		const char *number_cycles = string_number_cycles.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(number_cycles);
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("74964007", "SRT", "Others"));
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("74964007", "SRT", "Others"));
	bool_yes = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["others"]["result"]["yes"].get<bool>();
	bool_no = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["others"]["result"]["no"].get<bool>();
	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("72", "PrimageNB", "Number of cycles"));
		int num_number_cycles = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["others"]["number_of_cycles"]["value"].get<int>();
		std::string string_number_cycles = to_string(num_number_cycles);
		const char *number_cycles = string_number_cycles.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(number_cycles);

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("703503000", "SRT", "Name"));
		std::string string_name = j["treatment_and_evaluation"]["first_treatment"]["treatment_received"]["others"]["name"]["value"];
		const char *name = string_name.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(name);
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
}

doc->getTree().goUp();

doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("Evaluation", "SWEET", "Treatment evaluation"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("75", "PrimageNB", "First treatment completely finished"));
bool_yes = j["treatment_and_evaluation"]["treatment_evaluation"]["treatment_finished"]["result"]["yes"].get<bool>();
bool_no = j["treatment_and_evaluation"]["treatment_evaluation"]["treatment_finished"]["result"]["no"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setStringValue("Yes");
}
else if (bool_no == true) {
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C25638", "NCIt", "Reason for no completion"));
	std::string string_reason = j["treatment_and_evaluation"]["treatment_evaluation"]["treatment_finished"]["reason"]["value"]["value"];
	std::string other = "Other";
	if (string_reason == other) {
		std::string string_reason = j["treatment_and_evaluation"]["treatment_evaluation"]["treatment_finished"]["reason"]["other"]["value"];
		const char *reason = string_reason.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(reason);
	}
	else if (string_reason != other) {
		const char *reason = string_reason.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(reason);
	}

}

doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_DateTime, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C164340", "NCIt", "Date of evaluation"));
std::string string_ev_date = j["treatment_and_evaluation"]["treatment_evaluation"]["date_of_evaluation"]["value"];
std::string chars = "-T:.Z";
for (char c : chars) {
	string_ev_date.erase(std::remove(string_ev_date.begin(), string_ev_date.end(), c), string_ev_date.end());
}
std::string sub_string_ev_date = string_ev_date.substr(0, 12);
const char *ev_date = sub_string_ev_date.c_str();
doc->getTree().getCurrentContentItem().setObservationDateTime(ev_date);
doc->getTree().getCurrentContentItem().setStringValue(ev_date);

doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("363679005", "SRT", "Imaging "));
doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID10813", "RADLEX", "MRI"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID10813", "RADLEX", "MRI"));
bool_yes = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mri"]["evaluated"]["yes"].get<bool>();
bool_no = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mri"]["evaluated"]["no"].get<bool>();
not_available = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mri"]["evaluated"]["not_available"].get<bool>();
missing = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mri"]["evaluated"]["missing"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385658003", "SRT", "Done"));
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("706821004", "SRT", "Manufacturer and model"));
	std::string string_model = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mri"]["equipment"]["value"];
	const char *model = string_model.c_str();
	doc->getTree().getCurrentContentItem().setStringValue(model);
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_DateTime, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("410672004", "SRT", "Date"));
	std::string string_date = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mri"]["date"]["value"];
	std::string chars = "-T:.Z";
	for (char c : chars) {
		string_date.erase(std::remove(string_date.begin(), string_date.end(), c), string_date.end());
	}
	std::string sub_string_date = string_date.substr(0, 12);
	const char *date = sub_string_date.c_str();
	doc->getTree().getCurrentContentItem().setObservationDateTime(date);
	doc->getTree().getCurrentContentItem().setStringValue(date);


}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385660001", "SRT", "Not done"));
}
else if (not_available == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA7338-2", "LN", "Not available"));
}
else if (missing == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA14698-7", "LN", "Missing"));
}
doc->getTree().goUp();

doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID10321", "RADLEX", "CT"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID10321", "RADLEX", "CT"));
bool_yes = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["ct"]["evaluated"]["yes"].get<bool>();
bool_no = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["ct"]["evaluated"]["no"].get<bool>();
not_available = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["ct"]["evaluated"]["not_available"].get<bool>();
missing = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["ct"]["evaluated"]["missing"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385658003", "SRT", "Done"));
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("706821004", "SRT", "Manufacturer and model"));
	std::string string_model = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["ct"]["equipment"]["value"];
	const char *model = string_model.c_str();
	doc->getTree().getCurrentContentItem().setStringValue(model);
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("410672004", "SRT", "Date"));
	std::string string_date = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["ct"]["date"]["value"];
	std::string chars = "-T:.Z";
	for (char c : chars) {
		string_date.erase(std::remove(string_date.begin(), string_date.end(), c), string_date.end());
	}
	std::string sub_string_date = string_date.substr(0, 12);
	const char *date = sub_string_date.c_str();
	doc->getTree().getCurrentContentItem().setObservationDateTime(date);
	doc->getTree().getCurrentContentItem().setStringValue(date);

}
else if (bool_no == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("385660001", "SRT", "Not done"));
}
else if (not_available == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA7338-2", "LN", "Not available"));
}
else if (missing == true) {
	doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("LA14698-7", "LN", "Missing"));
}

doc->getTree().goUp();
doc->getTree().goUp();

//MIBG

doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID45807", "RADLEX", "MIBG entire body"));
doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID45807", "RADLEX", "MIBG entire body"));
bool_yes = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["evaluated"]["yes"].get<bool>();
bool_no = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["evaluated"]["no"].get<bool>();
not_available = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["evaluated"]["not_available"].get<bool>();
missing = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["evaluated"]["missing"].get<bool>();
if (bool_yes == true) {
	doc->getTree().getCurrentContentItem().setStringValue("Done");
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("706821004", "SRT", "Manufacturer and model"));
	std::string string_model = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["equipment"]["value"];
	const char *model = string_model.c_str();
	doc->getTree().getCurrentContentItem().setStringValue(model);

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_DateTime, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("410672004", "SRT", "Date"));
	std::string string_date = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["date"]["value"];
	std::string chars = "-T:.Z";
	for (char c : chars) {
		string_date.erase(std::remove(string_date.begin(), string_date.end(), c), string_date.end());
	}
	std::string sub_string_date = string_date.substr(0, 12);
	const char *date = sub_string_date.c_str();
	doc->getTree().getCurrentContentItem().setObservationDateTime(date);
	doc->getTree().getCurrentContentItem().setStringValue(date);

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("272394005", "SRT", "Technique"));
	std::string string_technique = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["technique"]["value"];
	const char *technique = string_technique.c_str();
	doc->getTree().getCurrentContentItem().setStringValue(technique);

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("408102007", "SRT", "Dose unit"));
	std::string string_dose_unit = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["dose_unit"]["value"];
	const char *dose_unit = string_dose_unit.c_str();
	doc->getTree().getCurrentContentItem().setStringValue(dose_unit);
	std::string unknown = "Unknown";
	if (dose_unit != unknown) {
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("260299005", "SRT", "Dose value"));
		int num_dose = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["dose"]["value"].get<int>();
		std::string string_dose = to_string(num_dose);
		const char *dose = string_dose.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(dose);
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C43385", "NCIt", "Image acquisition"));
	bool bool_other = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["image_acquisition"]["other"].get<bool>();
	bool bool_4h = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["image_acquisition"]["other"].get<bool>();
	//bool bool_24h = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["image_acquisition"]["other"].get<bool>();
	//const char *dose_unit = string_dose_unit.c_str();
	//doc->getTree().getCurrentContentItem().setStringValue(dose_unit);

	if (bool_other == true) {
		doc->getTree().getCurrentContentItem().setStringValue("Other");
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("130194", "DCM", "Time after injection in h"));
		int num_time_after = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["image_acquisition_other"]["value"].get<int>();
		std::string string_time_after = to_string(num_time_after);
		const char *time_after = string_time_after.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(time_after);

	}
	else if (bool_4h == true) {
		doc->getTree().getCurrentContentItem().setStringValue("4h");
	}
	else {
		doc->getTree().getCurrentContentItem().setStringValue("24h");
	}

	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("MTHU010322", "NCIt", "Uptake"));
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("MTHU010322", "LN", "Primary site"));
	std::string mibg_up_ps = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["uptake"]["primary_tumor"]["value"];
	
	if (mibg_up_ps == "Yes") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
	}
	else if (mibg_up_ps == "No") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (mibg_up_ps == "Not evaluable") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("fma303630", "FMA", "Skeleton"));
	if (mibg_up_ps == "Yes") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
	}
	else if (mibg_up_ps == "No") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (mibg_up_ps == "Not evaluable") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C12471", "NCIt", "Soft tissue"));
	string mibg_up_st = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["uptake"]["soft_tissue"]["value"];
	if (mibg_up_ps == "Yes") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
	}
	else if (mibg_up_ps == "No") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (mibg_up_ps == "Not evaluable") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("fma7197", "FMA", "Liver"));
	string mibg_up_liver = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["uptake"]["liver"]["value"];
	if (mibg_up_ps == "Yes") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
	}
	else if (mibg_up_ps == "No") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (mibg_up_ps == "Not evaluable") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID34290", "RADLEX", "Skin"));
	string mibg_up_skin = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["uptake"]["skin"]["value"];
	if (mibg_up_skin == "Yes") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
	}
	else if (mibg_up_skin == "No") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}
	else if (mibg_up_skin == "Not evaluable") {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("46", "PrimageNB", "Score SIOPEN"));


	/*bool_yes = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["score_siopen"]["result"]["yes"].get<bool>();
	bool_no = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["score_siopen"]["result"]["no"].get<bool>();
	if (bool_yes == true) {
		int num_score_siopen = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["mibg"]["score_siopen"]["value"]["value"].get<int>;
		std::string string_score_siopen = to_string(num_score_siopen);
		const char *score_siopen = string_score_siopen.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(score_siopen);

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setStringValue("No");
	}*/

}
	else if (not_available == true) {
	doc->getTree().getCurrentContentItem().setStringValue("Not available");
	}
	else {
	doc->getTree().getCurrentContentItem().setStringValue("Missing");
	}

	doc->getTree().goUp();
	doc->getTree().goUp();

	//PET

	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID10341", "RADLEX", "PET/CT"));
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID10341", "RADLEX", "PET/CT"));
	bool_yes = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["pet"]["evaluated"]["yes"].get<bool>();
	bool_no = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["pet"]["evaluated"]["no"].get<bool>();
	not_available = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["pet"]["evaluated"]["not_available"].get<bool>();
	missing = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["pet"]["evaluated"]["missing"].get<bool>();
	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setStringValue("Done");
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("706821004", "SRT", "Manufacturer and model"));
		checker = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["pet"]["equipment"]["value"].is_null();
		std::string string_model = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["pet"]["equipment"]["value"];
		const char *model = string_model.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(model);

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_DateTime, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("410672004", "SRT", "Date"));
		std::string string_date = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["pet"]["date"]["value"];
		std::string chars = "-T:.Z";
		for (char c : chars) {
			string_date.erase(std::remove(string_date.begin(), string_date.end(), c), string_date.end());
		}
		std::string sub_string_date = string_date.substr(0, 12);
		const char *date = sub_string_date.c_str();
		doc->getTree().getCurrentContentItem().setObservationDateTime(date);
		doc->getTree().getCurrentContentItem().setStringValue(date);

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("35321007", "SRT", "FDG - 18F(administered dose)"));
		int number_fdg_18f_dose = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["pet"]["fdg_18f"]["value"].get<int>();
		std::string string_number_fdg_18f_dose = to_string(number_fdg_18f_dose);
		const char *fdg_18f_dose = string_number_fdg_18f_dose.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(fdg_18f_dose);

		doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C71688", "NCIt", "Uptake"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("MTHU010322", "LN", "Primary site"));
		string pet_uptake_ps = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["pet"]["uptake"]["primary_tumor"]["value"];

		if (pet_uptake_ps == "Yes") {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("276625007", "SRT", "Primary site - Level"));
			string str_pet_uptake_ps_level = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["pet"]["uptake"]["primary_tumor_level"]["value"];
			const char *pet_uptake_ps_level = str_pet_uptake_ps_level.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(pet_uptake_ps_level);
			
		}
		else if (pet_uptake_ps == "No") {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
		}
		else if (pet_uptake_ps == "Not evaluable") {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
		}

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID5231", "RADLEX", "Skeleton metastasis"));
		string pet_uptake_metastasis = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["pet"]["uptake"]["metastasis_skeleton"]["value"];
		
		if (pet_uptake_metastasis == "Yes") {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("276625007", "SRT", "Skeleton metastasis - Level"));
			string str_pet_uptake_ps_level = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["pet"]["uptake"]["metastasis_skeleton_level"]["value"];
			const char *pet_uptake_ps_level = str_pet_uptake_ps_level.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(pet_uptake_ps_level);
			
		}
		else if (pet_uptake_metastasis == "No") {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
		}
		else if (pet_uptake_metastasis == "Not evaluable") {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
		}

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID5231", "RADLEX", "Soft tissue metastasis"));
		string pet_uptake_soft = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["pet"]["uptake"]["metastasis_soft_tissue"]["value"];
		if (pet_uptake_soft == "Yes") {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("276625007", "SRT", "Soft tissue metastasis - Level"));
			string str_pet_uptake_soft = j["treatment_and_evaluation"]["treatment_evaluation"]["imaging"]["pet"]["uptake"]["metastasis_soft_tissue_level"]["value"];
			const char *pet_uptake_soft = str_pet_uptake_soft.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(pet_uptake_soft);
		}
		else if (pet_uptake_soft == "No") {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
		}
		else if (pet_uptake_soft == "Not evaluable") {
			doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("RID39225", "RADLEX", "Not evaluable"));
		}
	}
		
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setStringValue("Not done");
	}
	else if (not_available == true) {
		doc->getTree().getCurrentContentItem().setStringValue("Not available");
	}
	else if (missing == true) {
		doc->getTree().getCurrentContentItem().setStringValue("Missing");
	}

	doc->getTree().goUp();
	doc->getTree().goUp();



	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C102869", "NCIt", "INRC(International Neuroblastoma Response Criteria)"));
	
	
	if(string_riskg == "Low" || string_riskg == "Intermediate"){
		doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("76", "PrimageNB", "Low/Intermediate risk patients"));
		doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("399687005", "SRT", "Primary tumor site"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("399687005", "SRT", "Primary tumor site"));
		bool_yes = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["primary_tumor"]["evaluated"]["yes"].get<bool>();
		bool_no = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["primary_tumor"]["evaluated"]["no"].get<bool>();
		if (bool_yes == true) {
			doc->getTree().getCurrentContentItem().setStringValue("Evaluated");
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C25755", "NCIt", "Response"));
			std::string string_response = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["primary_tumor"]["response"]["value"];
			const char *response = string_response.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(response);
		}
		else if (bool_no == true) {
			doc->getTree().getCurrentContentItem().setStringValue("Not evaluated");
		}

		doc->getTree().goUp();
		doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID5231", "RADLEX", "Metastasis"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C25755", "NCIt", "Response"));
		bool_yes = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["metastasis"]["evaluated"]["yes"].get<bool>();
		bool_no = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["metastasis"]["evaluated"]["no"].get<bool>();
		bool_unknown = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["metastasis"]["evaluated"]["not_applicable"].get<bool>();

		if (bool_yes == true) {
			doc->getTree().getCurrentContentItem().setStringValue("Evaluated");
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("fma30317", "FMA", "Bone"));
			std::string string_met_value = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["metastasis"]["low_or_intermediate_risk"]["bone"]["value"];
			const char *met_value = string_met_value.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(met_value);

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("D008198", "MESH", "Lymph nodes"));
			std::string string_lymph_nodes = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["metastasis"]["low_or_intermediate_risk"]["lymph_nodes"]["value"];
			const char *lymph_nodes = string_lymph_nodes.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(lymph_nodes);

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID38594", "RADLEX", "Bone marrow"));
			std::string string_bone_marrow = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["metastasis"]["low_or_intermediate_risk"]["bone_marrow"]["value"];
			const char *bone_marrow = string_met_value.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(bone_marrow);

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("fma7197", "FMA", "Liver"));
			std::string string_liver = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["metastasis"]["low_or_intermediate_risk"]["liver"]["value"];
			const char *liver = string_liver.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(liver);

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID34290", "RADLEX", "Skin"));
			std::string string_skin = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["metastasis"]["low_or_intermediate_risk"]["skin"]["value"];
			const char *skin = string_skin.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(skin);

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("200906047768068685", "IOBC", "CNS"));
			std::string string_cns = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["metastasis"]["low_or_intermediate_risk"]["cns"]["value"];
			const char *cns = string_cns.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(cns);

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("77", "PrimageNB", "Pleura lung"));
			std::string string_pleura_lung = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["metastasis"]["low_or_intermediate_risk"]["pleura_lung"]["value"];
			const char *pleura_lung = string_pleura_lung.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(pleura_lung);

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("74964007", "SRT", "Other"));
			std::string string_met_other = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["metastasis"]["low_or_intermediate_risk"]["other"]["value"];
			const char *met_other = string_met_other.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(met_other);
			doc->getTree().goUp();
			doc->getTree().goUp();
		}
		
		else if (bool_no == true) {
			doc->getTree().getCurrentContentItem().setStringValue("Not evaluated");

			doc->getTree().goUp();
			doc->getTree().goUp();
		}
		else if (bool_unknown == true) {
			doc->getTree().getCurrentContentItem().setStringValue("Not applicable");
			doc->getTree().goUp();
			doc->getTree().goUp();

		}
	}
	

	if (string_riskg == "High") {
		doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("76", "PrimageNB", "High risk patients"));
		doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("399687005", "SRT", "Primary tumor site"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("399687005", "SRT", "Primary tumor site"));
		bool_yes = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["primary_tumor"]["evaluated"]["yes"].get<bool>();
		bool_no = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["primary_tumor"]["evaluated"]["no"].get<bool>();
		if (bool_yes == true) {
			doc->getTree().getCurrentContentItem().setStringValue("Evaluated");
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C25755", "NCIt", "Response"));
			std::string string_response = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["primary_tumor"]["response"]["value"];
			const char *response = string_response.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(response);
		}
		else if (bool_no == true) {
			doc->getTree().getCurrentContentItem().setStringValue("Not evaluated");
		}

		doc->getTree().goUp();
		doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID5231", "RADLEX", "Metastasis"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C25755", "NCIt", "Response"));
		bool_yes = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["metastasis"]["evaluated"]["yes"].get<bool>();
		bool_no = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["metastasis"]["evaluated"]["no"].get<bool>();
		bool_unknown = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["metastasis"]["evaluated"]["not_applicable"].get<bool>();

		if (bool_yes == true) {
			doc->getTree().getCurrentContentItem().setStringValue("Evaluated");
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("fma30317", "FMA", "Bone"));
			std::string string_met_value = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["metastasis"]["high_risk"]["bone"]["value"];
			const char *met_value = string_met_value.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(met_value);

			
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID38594", "RADLEX", "Bone marrow"));
			std::string string_bone_marrow = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["metastasis"]["high_risk"]["bone_marrow"]["value"];
			const char *bone_marrow = string_met_value.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(bone_marrow);

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("fma7197", "FMA", "Liver"));
			std::string string_liver = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["metastasis"]["high_risk"]["liver"]["value"];
			const char *liver = string_liver.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(liver);

			
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("74964007", "SRT", "Other"));
			std::string string_met_other = j["treatment_and_evaluation"]["treatment_evaluation"]["response_evaluation"]["metastasis"]["high_risk"]["other"]["value"];
			const char *met_other = string_met_other.c_str();
			doc->getTree().getCurrentContentItem().setStringValue(met_other);
			doc->getTree().goUp();
			doc->getTree().goUp();
		}

		else if (bool_no == true) {
			doc->getTree().getCurrentContentItem().setStringValue("Not evaluated");

			doc->getTree().goUp();
			doc->getTree().goUp();
		}
		else if (bool_unknown == true) {
			doc->getTree().getCurrentContentItem().setStringValue("Not applicable");
			doc->getTree().goUp();
			doc->getTree().goUp();

		}
	}


	//doc->getTree().goUp();

	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("10079987", "MEDdra", "Minimal Residual Disease"));
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID38594", "RADLEX", "Bone marrow"));
	bool_yes = j["treatment_and_evaluation"]["treatment_evaluation"]["minimal_residual_disease"]["bone_marrow"]["evaluated"]["yes"].get<bool>();
	bool_no = j["treatment_and_evaluation"]["treatment_evaluation"]["minimal_residual_disease"]["bone_marrow"]["evaluated"]["no"].get<bool>();
	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setStringValue("Evaluated");

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID38594", "RADLEX", "Bone marrow  Method"));
		std::string string_techinique = j["treatment_and_evaluation"]["treatment_evaluation"]["minimal_residual_disease"]["bone_marrow"]["technique"]["value"];
		const char *techinique = string_techinique.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(techinique);

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID38594", "RADLEX", "Bone marrow  Response"));
		std::string string_response = j["treatment_and_evaluation"]["treatment_evaluation"]["minimal_residual_disease"]["bone_marrow"]["response"]["value"];
		const char *response = string_response.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(response);
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setStringValue("Not evaluated");
	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("78", "PrimageNB", "Periphereal blood"));
	bool_yes = j["treatment_and_evaluation"]["treatment_evaluation"]["minimal_residual_disease"]["peripheral_blood"]["evaluated"]["yes"].get<bool>();
	bool_no = j["treatment_and_evaluation"]["treatment_evaluation"]["minimal_residual_disease"]["peripheral_blood"]["evaluated"]["no"].get<bool>();
	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setStringValue("Evaluated");

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID38594", "RADLEX", "Bone marrow  Method"));
		std::string string_techinique = j["treatment_and_evaluation"]["treatment_evaluation"]["minimal_residual_disease"]["peripheral_blood"]["method"]["value"];
		const char *techinique = string_techinique.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(techinique);

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("RID38594", "RADLEX", "Bone marrow  Response"));
		std::string string_response = j["treatment_and_evaluation"]["treatment_evaluation"]["minimal_residual_disease"]["peripheral_blood"]["response"]["value"];
		const char *response = string_response.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(response);
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setStringValue("Not evaluated");
	}
	//############################      END TREATMENT AND EVALUATION #############################//
	

	//FOLLOW-UP
	
	doc->getTree().gotoNamedNode(DSRCodedEntryValue("1", "PrimageNB", "Neuroblastoma structured report"));
	
	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C16033", "NCIt", "Follow-up "));

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("81", "PrimageNB", "TVD"));
	bool_yes = j["follow_up"]["tvd"]["result"]["yes"].get<bool>();
	bool_no = j["follow_up"]["tvd"]["result"]["no"].get<bool>();
	bool_unknown = j["follow_up"]["tvd"]["result"]["unknown"].get<bool>();
	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));

	}


	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("80", "PrimageNB", "Poor responder"));
	bool_yes = j["follow_up"]["poor_responder"]["result"]["yes"].get<bool>();
	bool_no = j["follow_up"]["poor_responder"]["result"]["no"].get<bool>();
	bool_unknown = j["follow_up"]["poor_responder"]["result"]["unknown"].get<bool>();
	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

	}
	else if (bool_unknown == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));

	}

	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_DateTime, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("MTHU060967", "LN", "Date of end of treatment(Total)"));
	std::string string_end_date = j["follow_up"]["date_end_total_treatment"]["value"];
	chars = "-T:.Z";
	for (char c : chars) {
		string_end_date.erase(std::remove(string_end_date.begin(), string_end_date.end(), c), string_end_date.end());
	}
	std::string sub_string_end_date = string_end_date.substr(0, 12);
	const char *end_date = sub_string_end_date.c_str();
	doc->getTree().getCurrentContentItem().setObservationDateTime(end_date);
	doc->getTree().getCurrentContentItem().setStringValue(end_date);

	doc->getTree().goUp();
	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("246450006", "SRT", "Progression"));
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("246450006", "SRT", "Progression"));
	bool_yes = j["follow_up"]["progression"]["result"]["yes"].get<bool>();
	bool_no = j["follow_up"]["progression"]["result"]["no"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_DateTime, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("410672004", "SRT", "Date of first progression"));
		std::string string_dfp = j["follow_up"]["progression"]["date_first_progression"]["value"];
		chars = "-T:.Z";
		for (char c : chars) {
			string_dfp.erase(std::remove(string_dfp.begin(), string_dfp.end(), c), string_dfp.end());
		}
		std::string sub_string_dfp = string_dfp.substr(0, 12);
		const char *dfp = sub_string_dfp.c_str();
		doc->getTree().getCurrentContentItem().setObservationDateTime(dfp);
		doc->getTree().getCurrentContentItem().setStringValue(dfp);

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("410656007", "SRT", "Type of progression"));
		std::string string_top = j["follow_up"]["progression"]["type_of_progression"]["value"];
		const char *top = string_top.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(top);

		std::string local = "Local";
		std::string distal = "Distal";
		std::string combined = "Combined";

		if (top == distal || top == combined) {
			//
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("410656007", "SRT", "Distant site - Skeleton"));
			bool_yes = j["follow_up"]["progression"]["distant_site"]["skeleton"]["yes"].get<bool>();
			bool_no = j["follow_up"]["progression"]["distant_site"]["skeleton"]["no"].get<bool>();
			bool_unknown = j["follow_up"]["progression"]["distant_site"]["skeleton"]["unknown"].get<bool>();

			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
			}
			else if (bool_unknown == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
			}

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C12431", "NCIt", "Distant site - Bone marrow"));
			bool_yes = j["follow_up"]["progression"]["distant_site"]["bone_marrow"]["yes"].get<bool>();
			bool_no = j["follow_up"]["progression"]["distant_site"]["bone_marrow"]["no"].get<bool>();
			bool_unknown = j["follow_up"]["progression"]["distant_site"]["bone_marrow"]["unknown"].get<bool>();

			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
			}
			else if (bool_unknown == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
			}

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("Lymph nodes", "MESH", "Distant site - Lymph nodes"));
			bool_yes = j["follow_up"]["progression"]["distant_site"]["lymph_nodes"]["yes"].get<bool>();
			bool_no = j["follow_up"]["progression"]["distant_site"]["lymph_nodes"]["no"].get<bool>();
			bool_unknown = j["follow_up"]["progression"]["distant_site"]["lymph_nodes"]["unknown"].get<bool>();

			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
			}
			else if (bool_unknown == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
			}

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C4015", "NCIt", "Distant site -  CNS metastasis"));
			bool_yes = j["follow_up"]["progression"]["distant_site"]["cns_metastasis"]["yes"].get<bool>();
			bool_no = j["follow_up"]["progression"]["distant_site"]["cns_metastasis"]["no"].get<bool>();
			bool_unknown = j["follow_up"]["progression"]["distant_site"]["cns_metastasis"]["unknown"].get<bool>();

			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
			}
			else if (bool_unknown == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
			}

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C0153676", "OCHV", "Distant site - Lung metastasis"));
			bool_yes = j["follow_up"]["progression"]["distant_site"]["lung_metastasis"]["yes"].get<bool>();
			bool_no = j["follow_up"]["progression"]["distant_site"]["lung_metastasis"]["no"].get<bool>();
			bool_unknown = j["follow_up"]["progression"]["distant_site"]["lung_metastasis"]["unknown"].get<bool>();

			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
			}
			else if (bool_unknown == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
			}

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C17998", "NCIt", "Distant site - Unknown"));
			bool_yes = j["follow_up"]["progression"]["distant_site"]["unknown"]["yes"].get<bool>();
			bool_no = j["follow_up"]["progression"]["distant_site"]["unknown"]["no"].get<bool>();
			bool_unknown = j["follow_up"]["progression"]["distant_site"]["unknown"]["unknown"].get<bool>();

			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
			}
			else if (bool_unknown == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
			}

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("74964007", "SRT", "Distant site - Other"));
			bool_yes = j["follow_up"]["progression"]["distant_site"]["other"]["result"]["yes"].get<bool>();
			bool_no = j["follow_up"]["progression"]["distant_site"]["other"]["result"]["no"].get<bool>();
			bool_unknown = j["follow_up"]["progression"]["distant_site"]["other"]["result"]["unknown"].get<bool>();

			if (bool_yes == true) {
				std::string string_other = j["follow_up"]["progression"]["distant_site"]["other"]["value"]["value"];
				const char *other = string_other.c_str();
				doc->getTree().getCurrentContentItem().setStringValue(other);

			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setStringValue("No");
			}
			else if (bool_unknown == true) {
				doc->getTree().getCurrentContentItem().setStringValue("Unknown");
			}
		}


	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));

	}


	doc->getTree().goUp();
	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("OGMS_0000105", "OGMS", "Relapse"));
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("OGMS_0000105", "OGMS", "Relapse"));
	bool_yes = j["follow_up"]["relapse"]["result"]["yes"].get<bool>();
	bool_no = j["follow_up"]["relapse"]["result"]["no"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setStringValue("Yes");

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_DateTime, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("410672004", "SRT", "Date of first relapse"));
		std::string string_dfp = j["follow_up"]["relapse"]["date_first_relapse"]["value"];
		chars = "-T:.Z";
		for (char c : chars) {
			string_dfp.erase(std::remove(string_dfp.begin(), string_dfp.end(), c), string_dfp.end());
		}
		std::string sub_string_dfp = string_dfp.substr(0, 12);
		const char *dfp = sub_string_dfp.c_str();
		doc->getTree().getCurrentContentItem().setObservationDateTime(dfp);
		doc->getTree().getCurrentContentItem().setStringValue(dfp);

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_belowCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("410656007", "SRT", "Type of relapse"));
		std::string string_top = j["follow_up"]["relapse"]["type_of_relapse"]["value"];
		const char *top = string_top.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(top);

		std::string local = "Local";
		std::string distal = "Distal";
		std::string combined = "Combined";

		if (top == distal || top == combined) {
			//
			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("410656007", "SRT", "Distant site - Skeleton"));
			bool_yes = j["follow_up"]["relapse"]["distant_site"]["skeleton"]["yes"].get<bool>();
			bool_no = j["follow_up"]["relapse"]["distant_site"]["skeleton"]["no"].get<bool>();
			bool_unknown = j["follow_up"]["relapse"]["distant_site"]["skeleton"]["unknown"].get<bool>();

			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
			}
			else if (bool_unknown == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
			}

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C12431", "NCIt", "Distant site - Bone marrow"));
			bool_yes = j["follow_up"]["relapse"]["distant_site"]["bone_marrow"]["yes"].get<bool>();
			bool_no = j["follow_up"]["relapse"]["distant_site"]["bone_marrow"]["no"].get<bool>();
			bool_unknown = j["follow_up"]["relapse"]["distant_site"]["bone_marrow"]["unknown"].get<bool>();

			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
			}
			else if (bool_unknown == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
			}

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("Lymph nodes", "MESH", "Distant site - Lymph nodes"));
			bool_yes = j["follow_up"]["relapse"]["distant_site"]["lymph_nodes"]["yes"].get<bool>();
			bool_no = j["follow_up"]["relapse"]["distant_site"]["lymph_nodes"]["no"].get<bool>();
			bool_unknown = j["follow_up"]["relapse"]["distant_site"]["lymph_nodes"]["unknown"].get<bool>();

			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
			}
			else if (bool_unknown == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
			}

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C4015", "NCIt", "Distant site -  CNS metastasis"));
			bool_yes = j["follow_up"]["relapse"]["distant_site"]["cns_metastasis"]["yes"].get<bool>();
			bool_no = j["follow_up"]["relapse"]["distant_site"]["cns_metastasis"]["no"].get<bool>();
			bool_unknown = j["follow_up"]["relapse"]["distant_site"]["cns_metastasis"]["unknown"].get<bool>();

			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
			}
			else if (bool_unknown == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
			}

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C0153676", "OCHV", "Distant site - Lung metastasis"));
			bool_yes = j["follow_up"]["relapse"]["distant_site"]["lung_metastasis"]["yes"].get<bool>();
			bool_no = j["follow_up"]["relapse"]["distant_site"]["lung_metastasis"]["no"].get<bool>();
			bool_unknown = j["follow_up"]["relapse"]["distant_site"]["lung_metastasis"]["unknown"].get<bool>();

			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
			}
			else if (bool_unknown == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
			}

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C17998", "NCIt", "Distant site - Unknown"));
			bool_yes = j["follow_up"]["relapse"]["distant_site"]["unknown"]["yes"].get<bool>();
			bool_no = j["follow_up"]["relapse"]["distant_site"]["unknown"]["no"].get<bool>();
			bool_unknown = j["follow_up"]["relapse"]["distant_site"]["unknown"]["unknown"].get<bool>();

			if (bool_yes == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));

			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
			}
			else if (bool_unknown == true) {
				doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("261665006", "SRT", "Unknown"));
			}

			doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
			doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("74964007", "SRT", "Distant site - Other"));
			bool_yes = j["follow_up"]["relapse"]["distant_site"]["other"]["result"]["yes"].get<bool>();
			bool_no = j["follow_up"]["relapse"]["distant_site"]["other"]["result"]["no"].get<bool>();
			bool_unknown = j["follow_up"]["relapse"]["distant_site"]["other"]["result"]["unknown"].get<bool>();

			if (bool_yes == true) {
				std::string string_other = j["follow_up"]["relapse"]["distant_site"]["other"]["value"]["value"];
				const char *other = string_other.c_str();
				doc->getTree().getCurrentContentItem().setStringValue(other);

			}
			else if (bool_no == true) {
				doc->getTree().getCurrentContentItem().setStringValue("No");
			}
			else if (bool_unknown == true) {
				doc->getTree().getCurrentContentItem().setStringValue("Unknown");
			}
		}


	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setStringValue("No");

	}

	doc->getTree().goUp();
	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("10039801", "MEDdra", "Second malignancy"));
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Code, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("10039801", "MEDdra", "Second malignancy"));
	bool_yes = j["follow_up"]["second_malignancy"]["result"]["yes"].get<bool>();
	bool_no = j["follow_up"]["second_malignancy"]["result"]["no"].get<bool>();
	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373066001", "SRT", "Yes"));
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_DateTime, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("432213005", "SRT", "Diagnosis date"));
		std::string string_ddate = j["follow_up"]["second_malignancy"]["diagnosis_date"]["value"];
		chars = "-T:.Z";
		for (char c : chars) {
			string_ddate.erase(std::remove(string_ddate.begin(), string_ddate.end(), c), string_ddate.end());
		}
		std::string sub_string_ddate = string_ddate.substr(0, 12);
		const char *ddate = sub_string_ddate.c_str();
		doc->getTree().getCurrentContentItem().setObservationDateTime(ddate);
		doc->getTree().getCurrentContentItem().setStringValue(ddate);

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("LP207876-6", "LN", "Type of tumor"));
		std::string string_ttop = j["follow_up"]["second_malignancy"]["type_of_tumor"]["value"];
		const char *ttop = string_ttop.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(ttop);
	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setCodeValue(DSRCodedEntryValue("373067005", "SRT", "No"));
	}




	doc->getTree().goUp();
	doc->getTree().addContentItem(DSRTypes::RT_contains, DSRTypes::VT_Container, DSRTypes::AM_belowCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("C115767", "NCIt", "Current status"));
	doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
	doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("398070004", "SRT", "State"));
	bool_yes = j["follow_up"]["current_status"]["state"]["alive"].get<bool>();
	bool_no = j["follow_up"]["current_status"]["state"]["death"].get<bool>();

	if (bool_yes == true) {
		doc->getTree().getCurrentContentItem().setStringValue("Alive");
		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_DateTime, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("79", "PrimageNB", "Date of last visit"));
		std::string string_date_last_visit = j["follow_up"]["current_status"]["date_last_visit"]["value"];
		chars = "-T:.Z";
		for (char c : chars) {
			string_date_last_visit.erase(std::remove(string_date_last_visit.begin(), string_date_last_visit.end(), c), string_date_last_visit.end());
		}
		std::string sub_string_date_last_visit = string_date_last_visit.substr(0, 12);
		const char *date_last_visit = sub_string_date_last_visit.c_str();
		doc->getTree().getCurrentContentItem().setObservationDateTime(date_last_visit);
		doc->getTree().getCurrentContentItem().setStringValue(date_last_visit);

	}
	else if (bool_no == true) {
		doc->getTree().getCurrentContentItem().setStringValue("Death");

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_DateTime, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("399753006", "SRT", "Date of death"));
		std::string string_date_of_death = j["follow_up"]["current_status"]["date_of_death"]["value"];
		chars = "-T:.Z";
		for (char c : chars) {
			string_date_of_death.erase(std::remove(string_date_of_death.begin(), string_date_of_death.end(), c), string_date_of_death.end());
		}
		std::string sub_string_date_of_death = string_date_of_death.substr(0, 12);
		const char *date_of_death = sub_string_date_of_death.c_str();
		doc->getTree().getCurrentContentItem().setObservationDateTime(date_of_death);
		doc->getTree().getCurrentContentItem().setStringValue(date_of_death);

		doc->getTree().addContentItem(DSRTypes::RT_hasObsContext, DSRTypes::VT_Text, DSRTypes::AM_afterCurrent);
		doc->getTree().getCurrentContentItem().setConceptName(DSRCodedEntryValue("184305005", "SRT", "Cause of death"));
		std::string string_cause_of_death = j["follow_up"]["current_status"]["cause_of_death"]["value"];
		const char *cause_of_death = string_cause_of_death.c_str();
		doc->getTree().getCurrentContentItem().setStringValue(cause_of_death);
	}



	
	//#############################                END               ######################3

	

	DcmFileFormat *fileformat = new DcmFileFormat();
	DcmDataset *dataset = NULL;
	if (fileformat != NULL)
		dataset = fileformat->getDataset();
	if (dataset != NULL)
	{
		if (doc->write(*dataset).good())
			fileformat->saveFile("/folder/eform.dcm", EXS_LittleEndianExplicit);
			
	}
	delete fileformat;

	delete doc;

	return 0;

}
