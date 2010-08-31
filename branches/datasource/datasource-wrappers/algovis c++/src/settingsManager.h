#ifndef SETTINGS_MANAGER_H_
#define SETTINGS_MANAGER_H_



namespace Algovis
{
	

	// TODO Provide a generic settings class as we'll have many more settings in the future
	class SettingsManager
	{

	public:
		static SettingsManager* GetInstance();
			
		void EnableConstructionReporting(bool);
		bool ConstructionReportingEnabled();

		void EnableCopyConstructionReporting(bool);
		bool CopyConstructionReportingEnabled();

		void EnableCopyAssignmentReporting(bool);
		bool CopyAssignmentReportingEnabled();

		void EnableDestructionReporting(bool);
		bool DestructionReportingEnabled();


	private:
		static SettingsManager* settingsMgrInstance;
		SettingsManager();


		bool constructionReportingEnabled, copyConstructionReportingEnabled;
		bool copyAssignmentReportingEnabled, destructionReportingEnabled;


	};



}







#endif