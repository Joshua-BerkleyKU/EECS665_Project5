set breakpoint pending on
set confirm off
file ./cshantyc
break cshanty::Err::report
commands
	where
end
break cshanty::InternalError::InternalError
commands
	where
end
break cshanty::ToDoError::ToDoError
commands
	where
end

define p5
  set args p5_tests/$arg0.cshantyc -c
  run
end
