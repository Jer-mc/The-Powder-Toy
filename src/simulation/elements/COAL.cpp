#include "simulation/ElementCommon.h"

int Element_COAL_update(UPDATE_FUNC_ARGS);
int Element_COAL_graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_COAL()
{
	Identifier = "DEFAULT_PT_COAL";
	Name = "COAL";
	Colour = PIXPACK(0x222222);
	MenuVisible = 1;
	MenuSection = SC_SOLIDS;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.0f;
	HotAir = 0.0f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 20;
	PhotonReflectWavelengths = 0x00000000;

	Weight = 100;

	HeatConduct = 200;
	Description = "Coal, Burns slower than CHRC. Gets red when hot.";

	Properties = TYPE_SOLID;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	DefaultProperties.life = 1250;
	DefaultProperties.tmp = 50;

	Update = &Element_COAL_update;
	Graphics = &Element_COAL_graphics;
}

int Element_COAL_update(UPDATE_FUNC_ARGS)
{
	if (parts[i].life<=0) {
		sim->create_part(i, x, y, PT_FIRE);
		return 1;
	} else if (parts[i].life < 150) {
		parts[i].life--;
		sim->create_part(-1, x + RNG::Ref().between(-1, 1), y + RNG::Ref().between(-1, 1), PT_FIRE);
	}
	if (parts[i].type == PT_COAL)
	{
		if ((sim->pv[y/CELL][x/CELL] > 4.3f)&&parts[i].tmp>40)
			parts[i].tmp=39;
		else if (parts[i].tmp<40&&parts[i].tmp>0)
			parts[i].tmp--;
		else if (parts[i].tmp<=0) {
			sim->part_change_type(i, x, y, PT_BCOL);
			return 1;
		}
	}
	if(parts[i].temp > parts[i].tmp2)
		parts[i].tmp2 = parts[i].temp;

	if ((parts[i].temp > 4500)) {
		int r, rx, ry;
		float cxy = 0;
		for (rx = -2; rx < 3; rx++)
			for (ry = -2; ry < 3; ry++)
				if (BOUNDS_CHECK && (rx || ry))
				{
					r = pmap[y + ry][x + rx];
					if (!r)
						continue;
					if (TYP(r) == PT_COCH)
					{
						if (RNG::Ref().chance(3, 1))
						{
							sim->create_part(i, x, y, PT_COCA);
							sim->kill_part(ID(r));

						}
					}
				}
	}

	return 0;
}

constexpr float FREQUENCY = 3.1415/(2*300.0f-(300.0f-200.0f));

int Element_COAL_graphics(GRAPHICS_FUNC_ARGS)
 //Both COAL and Broken Coal
{
	*colr += (cpart->tmp2-2950.15f)/3;

	if (*colr > 170)
		*colr = 170;
	if (*colr < *colg)
		*colr = *colg;

	*colg = *colb = *colr;

	// ((cpart->temp-295.15f) > 300.0f-200.0f)
	if (cpart->temp > 3950.15f)
	{
		//  q = ((cpart->temp-295.15f)>300.0f)?300.0f-(300.0f-200.0f):(cpart->temp-295.15f)-(300.0f-200.0f);
		int q = (cpart->temp > 5950.15f) ? 2000.0f : cpart->temp - 3950.15f;

		*colr += sin(FREQUENCY*q) * 2260;
		*colg += sin(FREQUENCY*q*4.55 + 3.14) * 34;
		*colb += sin(FREQUENCY*q*2.22 + 3.14) * 64;
	}
	return 0;
}
