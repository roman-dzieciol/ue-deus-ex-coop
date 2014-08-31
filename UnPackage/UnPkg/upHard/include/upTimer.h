// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	upTimer.h
// ============================================================================
#ifndef UPTIMER_H
#define UPTIMER_H


#ifdef WIN32
// ============================================================================
// upTimer
// ============================================================================
class upTimer
{
public:
	typedef float	tmult;
	typedef float	ttime;

	upTimer()
	{
		if( Frequency == 0 )
		{
			Init();
		}
		Start();
	}

	void Init()
	{
		CalcFrequency();
		CalcMultiplier();
	}

	void Start()
	{
		StartTicks = 0;
		SavedTicks = 0;
		GetTicks(StartTicks);
	}

	ttime Time()
	{
		qword total = SavedTicks;
		if( StartTicks != 0 )
		{
			qword counter;
			GetTicks(counter);
			total += counter-StartTicks;
		}
		return static_cast<ttime>(total * Multiplier);
	}

	void Pause()
	{
		if( StartTicks != 0 )
		{
			qword counter;
			GetTicks(counter);
			SavedTicks += counter - StartTicks;
			StartTicks = 0;
		}
		else
		{
			//wxLogMessage( wxT("%s :: timer already paused!"), __FUNCTION__ );
		}
	}

	void Resume()
	{
		if( StartTicks == 0 )
		{
			GetTicks(StartTicks);
		}
		else
		{
			//wxLogMessage( wxT("%s :: timer already resumed!"), __FUNCTION__ );
		}
	}


protected:
	void CalcFrequency()
	{
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&Frequency));
	}

	void CalcMultiplier()
	{
		Multiplier = static_cast<tmult>( Unit / static_cast<double>(Frequency)  );
	}

	void GetTicks( qword& counter )
	{
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&counter));
	}


protected:
	qword	StartTicks;				// Counter ticks at last Start() or Resume() 
	qword	SavedTicks;				// Total ticks stored by Pause() 
	static	tmult	Multiplier;		// Multiply ticks by this to get time in Unit's
	static	qword	Frequency;		// Clock frequency
	static	double	Unit;			// Time unit, 1 = s, 1000 = ms
};

#define wxStopWatch upTimer
#endif // WIN32


// ============================================================================
// Defs
// ============================================================================
#define UP_TIMER(var)					wxStopWatch var;
#define UP_TIMER_STATIC(var)				static wxStopWatch var; var.Resume();
#define UP_TIMER_STR(var)					wxString::Format( wxT("%.2fms"), static_cast<float>(var.Time()) ).c_str()
#define	UP_TIMER_START(var)					var.Start()	
#define	UP_TIMER_PAUSE(var)					var.Pause()	
#define	UP_TIMER_RESUME(var)				var.Resume()	
#define UP_TIMER_LOGF(var,start)			var.Pause(); upLogMessage(wxString::Format( wxT("%hs %s"), __FUNCTION__, UP_TIMER_STR(var))); if( start ) var.Start();
#define UP_TIMER_LOGV(var,txt,start)		var.Pause(); upLogMessage(wxString::Format( wxT("%s %s"), UP_TIME_STR(var), txt)); if( start ) var.Start();
#define UP_TIMER_VLOG(var,txt,start)		var.Pause(); upLogMessage(wxString::Format( wxT("%s %s"), txt, UP_TIMER_STR(var))); if( start ) var.Start();
#define UP_TIMER_STAT(var,stat)				var.Pause(); UPG_Stats.stat += var.Time();
	


#endif UPTIMER_H
// ============================================================================
//	The End.
// ============================================================================