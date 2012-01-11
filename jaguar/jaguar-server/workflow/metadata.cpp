#include "metadata.h"
#include <typeinfo>
#include <stdlib.h>
#include <fstream>
#include <dirent.h>
#include "metadata/entitynotfoundexception.h"
#include "metadata/servicetask.h"
#include "metadata/activitycommon.h"
#include "metadata/task.h"
#include "metadata/startevent.h"
#include "metadata/endevent.h"
#include "metadata/sequenceflow.h"
#include "metadata/manualtask.h"

using namespace std;
using namespace dbjaguar;
using namespace cache;

void loadConnectors(Connection* con, ProcessDefinition* def) {
    Logger* log = getLogger(NULL);
    try {
        if (log->isDebug()) log->debug("Loading connectors for definition: " + def->getDefinitionName());

        long iddef = def->getId();
        string sql = "SELECT id, connectorname, idtasksource, idtasktarget, ideventsource, ideventtarget, sourcetype, targettype, conditiontype, expression, quantity, idprocessdef FROM sequenceflows "
                "where idprocessdef = " + toString(iddef);
        ResultSet* rs = con->executeQuery(sql.c_str());

        while ((*rs)++) {
            SequenceFlow* sequence = new SequenceFlow();
            long *id = (long*) rs->get("id");
            sequence->setId(*id);
            string* connectorName = (string*) rs->get("connectorname");
            sequence->setConnectorName(*connectorName);
            //sequence->setConditionExpression((string*)rs->get("expression"));
            ConditionType* conditionType = static_cast<ConditionType*>(rs->get("conditiontype"));
            sequence->setConditionType(*conditionType);

            ConnectorType* sourceType = static_cast<ConnectorType*>(rs->get("sourcetype"));
            if ((*sourceType) == TASK_CTTYPE) {
                long* idSource = (long*)rs->get("idtasksource");
                ActivityCommon* activity = def->getActivity(*idSource);
                if (activity->getActivityType() == TASK_ACTIVITYTYPE) {
                    sequence->setTaskSource((Task*)activity);
                    sequence->setEventSource(NULL);
                    ((Task*)activity)->addSequenceFlow(sequence);
                }
                free(idSource);
            } else {
                long* idSource = (long*)rs->get("ideventsource");
                sequence->setTaskSource(NULL);
                CommonEvent* event = def->getEvent(*idSource);
                sequence->setEventSource(event);

                event->addSequenceFlow(sequence);
                free(idSource);
            }

            ConnectorType* targetType = static_cast<ConnectorType*>(rs->get("targettype"));
            if ((*targetType) == TASK_CTTYPE) {
                long* idTarget = (long*)rs->get("idtasktarget");
                ActivityCommon* activity = def->getActivity(*idTarget);
                if (activity->getActivityType() == TASK_ACTIVITYTYPE) {
                    sequence->setTaskTarget((Task*)activity);
                    sequence->setEventTarget(NULL);
                }
                free(idTarget);
            } else {
                long* idTarget = (long*)rs->get("ideventtarget");
                sequence->setTaskSource(NULL);
                CommonEvent* event = def->getEvent(*idTarget);
                sequence->setEventTarget(event);
                free(idTarget);
            }

            long *quantity = (long*) rs->get("quantity");
            sequence->setQuantity(*quantity);

            delete(connectorName);
            free(conditionType);
            free(quantity);
            free(targetType);
            free(sourceType);
            free(id);
        }
        rs->close();
        delete(rs);
    } catch (DBException e) {
        log->error(e.what());
    }
    delete(log);
}

vector<CommonEvent*>* loadEvents(Connection* con, ProcessDefinition* def) {
    Logger* log = getLogger(NULL);
    try {
        if (log->isDebug()) log->debug("Loading events definition for: " + def->getDefinitionName());

        vector<CommonEvent*>* res = new vector<CommonEvent*>();

        long idDef = def->getId();
        string sql = "SELECT id, eventtype, idpool, idprocessdef FROM events e "
                "where idprocessdef = " + toString(idDef);
        ResultSet* rs = con->executeQuery(sql.c_str());

        while ((*rs)++) {
            long *id = (long*) rs->get("id");
            EventType* eventType = static_cast<EventType*>(rs->get("eventtype"));

            CommonEvent* event;
            switch ((*eventType)) {
                case START_EVENTTYPE:
                    event = new StartEvent();
                    break;
                case END_EVENTTYPE:
                    event = new EndEvent();
                    break;
            }
            event->setId(*id);
            res->push_back(event);

            free(eventType);
            free(id);
        }
        delete(log);
        rs->close();
        delete(rs);
        return res;
    } catch (DBException e) {
        log->error(e.what());
    }
    delete(log);
}

vector<ActivityCommon*>* loadTasks(Connection* con, ProcessDefinition* def) {
    Logger* log = getLogger(NULL);
    try {
        if (log->isDebug()) log->debug("Loading tasks definition for: " + def->getDefinitionName());

        vector<ActivityCommon*>* res = new vector<ActivityCommon*>();

        long idDef = def->getId();
        string sql = "SELECT t.id, t.taskname, "
                "t.tasktype, a.activitytype, a.startquantity, a.looptype FROM task t inner join activities a on t.idactivity = a.id "
                "where a.idprocessdef = " + toString(idDef);
        ResultSet* rs = con->executeQuery(sql.c_str());

        while ((*rs)++) {
            long *id = (long*) rs->get("id");
            string* taskName = (string*) rs->get("taskname");
            TaskType* taskType = static_cast<TaskType*> (rs->get("tasktype"));
            ActivityType* actType = static_cast<ActivityType*> (rs->get("activitytype"));
            int *startQuantity = (int*) rs->get("startquantity");
            LoopType* loopType = static_cast<LoopType*> (rs->get("looptype"));

            Task* task;
            switch ((*taskType)) {
                case SERVICE_TASKTYPE:
                    task = new ServiceTask();
                    break;
                case MANUAL_TASKTYPE:
                    task = new ManualTask();
                    break;
            }
            task->setId(*id);
            task->setActivityType(*actType);
            task->setLoopType(*loopType);
            task->setStartQuantity(*startQuantity);
            task->setTaskName(*taskName);
            task->setTaskType(*taskType);
            res->push_back(task);

            free(actType);
            free(loopType);
            free(startQuantity);
            free(taskType);
            delete(taskName);
            free(id);
        }
        delete(log);
        rs->close();
        delete(rs);
        return res;
    } catch (DBException e) {
        log->error(e.what());
    }
    delete(log);
}

void unloadProcessDefinitions() {
    CacheGroup* cache = getGlobalCache("METADATA");

    map<long, ProcessDefinition*>* defs = (map<long, ProcessDefinition*>*)cache->get(string("PROCESSDEFINITIONS"));
    for (map<long, ProcessDefinition*>::iterator iter = defs->begin(); iter != defs->end(); iter++) {
        delete(iter->second);
    }
    defs->clear();
}

void loadProcessDefinitions(Connection* con) {
    Logger* log = getLogger(NULL);

    if (log->isInfo()) log->info("Loading process definitions");

    CacheGroup* cache = getGlobalCache("METADATA");
    ResultSet* rs = con->executeQuery("SELECT id, defname, processType, masterent FROM processdef");

    map<long, ProcessDefinition*>* mapDefinitions = new map<long, ProcessDefinition*>();
    while ((*rs)++) {
        long* id = (long*) rs->get(0);
        string* defname = (string*) rs->get(1);
        short int* processType = static_cast<short int*> (rs->get(2));
        string* masterent = (string*) rs->get(3);
        ProcessDefinition* def = new ProcessDefinition();
        def->setProcessType((ProcessType)(*processType));
        //def->setProperties(vector<Property*>* _properties);
        //def->setPools(vector<Pool*>* _pools);
        def->setDefinitionName(*defname);
        def->setId(*id);
        def->setMasterEntity(*masterent);

        def->setActivities(loadTasks(con, def));
        def->setEvents(loadEvents(con, def));
        loadConnectors(con, def);
        if (log->isDebug()) log->debug("Adding process definition: " + *defname);
        mapDefinitions->insert(pair<long, ProcessDefinition*>(*id, def));

        delete(defname);
        delete(masterent);
        free(processType);
        free(id);
    }

    rs->close();

    cache->add(string("PROCESSDEFINITIONS"), mapDefinitions);

    delete(rs);
    delete(log);
}

ProcessDefinition* getProcessDefinition(long id) throw (WorkflowException) {
    CacheGroup* group = getGlobalCache("METADATA");
    map<long, ProcessDefinition*>* definitions = (map<long, ProcessDefinition*>*)group->get("PROCESSDEFINITIONS");
    if (!definitions) {
        throw new WorkflowException(string("The process definition does not exist").c_str());
    }
    ProcessDefinition* def = definitions->find(id)->second;
    if (!def) {
        throw new WorkflowException(string("The process definition does not exist").c_str());
    }
    return def;
}

void deserialize(string fileName, void** readed) {
    ifstream file;
    file.open(fileName.c_str(), ios::binary);
    file.read((char*) readed, sizeof (*readed));
    file.close();
}

void serialize(string filename, void* file) {
    ofstream ofile;
    ofile.open(filename.c_str(), ios::binary);
    ofile.write((char*) file, sizeof (file));
    ofile.close();
}

const char* EntityNotFoundException::what() const throw () {
    return message->c_str();
}

