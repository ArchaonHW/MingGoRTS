---
status: blocked
---

# BMad Build Auto Result

Status: blocked
Blocking condition: working tree dirty with in-flight parallel-session
work — N-1 (spec-n1-historian-report.md status: in-progress,
Gameplay/HistorianReport.{h,cpp}, Examples/HistorianReportTest.cpp,
CMakeLists.txt registration already staged) plus unrelated modifications
(DevAssistantSmoke.cpp, IntelligentSuggestion.cpp, SquadTemplate.h,
narrative-design.md). auto-dev-workflow.md hard gate requires a clean
working tree before PICK; picking N-1 would collide with the parallel
run, and picking N-3+ would mix another session's uncommitted files into
this run's verification/commit scope.

Queue snapshot: N-1 in-flight (parallel), N-2 draft spec exists,
N-3..N-6 untouched queued.

Resolution paths: (a) let the parallel run commit N-1 first, then
re-dispatch; (b) user explicitly assigns N-3 or later to this session,
acknowledging the dirty-tree gate is waived for unrelated files.
